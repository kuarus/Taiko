#include "ScenePlay.h"
#include "Drawer.h"
#include "define.h"
#include "Songs.h"
#include "Sound.h"
#include "Bullet.h"


static const int WAIT_APPEAR_TIME = 40;
static const int MUSIC_VOL = 80;
static const int MAX_CODE = 128;
static const int OFF_SET = 118;
static const int RATIO = 6;
int a = 0;


ScenePlay::ScenePlay( int select, SongsPtr songs ) :
_state( STATE::STATE_NORMAL ),
_count( 0 ),
_old_position( 0 ),
_play_state( PLAY_STATE::PLAY_STATE_WAIT ) {
	_bg_image[ STATE::STATE_NORMAL] = Drawer::loadGraph( "Resource/img/bg.png" );
	_bg_image[ STATE::STATE_HOT ] = Drawer::loadGraph( "Resource/img/bg_clear.png" );
	_bg_image[ STATE::STATE_CLEAR ] = Drawer::loadGraph( "Resource/img/bg_hot.png" );
	_bg_image[ STATE::STATE_MISS ] = Drawer::loadGraph( "Resource/img/bg_miss.png" );
	_bg_image[ STATE::STATE_MISS_CLEAR ] = Drawer::loadGraph( "Resource/img/bg_miss_clear.png" );
	_taiko_image = Drawer::loadGraph( "Resource/img/notes.png" );
	Songs::SONG music_data = songs->getSongData( select );
	std::string music_file = music_data.directory + "/" + songs->getMusicFileName( select );
	_music = Sound::load( music_file.c_str( ) );
	_title = songs->getTitle( select );
	_code_list = songs->getCode( select );
 	_offset = songs->getOffset( select );
	Drawer::changeFont( H_FONT );
	_bpm = songs->getBpm( select );
	createBullet( );
}


ScenePlay::~ScenePlay( ) {
	for ( int i = 0; i < STATE::MAX_STATE; i++ ) {
		Drawer::deleteGraph( _bg_image[ i ] );
	}
	Drawer::deleteGraph( _taiko_image );
	Sound::stop( _music );
	Sound::destroy( _music );
}

void ScenePlay::update( GamePtr game ) {
	switch( _play_state ) {
	case PLAY_STATE::PLAY_STATE_WAIT:
		if ( game->isPushKey( Game::KEY::KEY_SPACE ) ) {
			_play_state = PLAY_STATE::PLAY_STATE_INIT;
		}
		break;
	case PLAY_STATE::PLAY_STATE_INIT:
		initialize( );
		break;
	case PLAY_STATE::PLAY_STATE_PLAY:
		updatePlay( game );
		break;
	case PLAY_STATE::PLAY_STATE_END:
		break;
	}

	if ( game->isBack( ) ) {
		game->setScene( Game::SCENE::SCENE_SONG_SELECT );
	}
}

void ScenePlay::draw( ) {
	drawBg( );
	drawBarLine( );
	drawBullet( );
	drawTitle( );
	if ( _play_state == PLAY_STATE::PLAY_STATE_WAIT ) {
		Drawer::drawString( 180, 240, "Pleas push SPACE key  start" );
	}
	Drawer::drawString( 0, 0, "%d", a );
}

void ScenePlay::initialize( ) {
	Sound::playSound( _music, false );
	Sound::changeVol( MUSIC_VOL, _music );
	_play_state = PLAY_STATE::PLAY_STATE_PLAY;
}

void ScenePlay::updatePlay( GamePtr game ) {
	int music_time = Sound::getTime( _music );
	int idx = ( music_time * 106 / _bpm  + _offset / 15 ) * RATIO;
	//if ( music_time == 1000 ) {
	//	bpm = MAX_CODE * 60 
	//}
	a = idx;
	updateBullet( idx, game );

	_count++;
	_count %= WAIT_APPEAR_TIME;
}

void ScenePlay::updateBullet( int idx, GamePtr game ) {
	std::list< BulletPtr >::iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		BulletPtr bullet = *ite;
		if ( !bullet ) {
			continue;
		}
		bullet->update( idx, game );
		if ( bullet->isOutSideScreen( ) ) {
			ite = _bullets.erase( ite );
			continue;
		}
		ite++;
	}
}

void ScenePlay::drawBg( ) {
	int width = 512;
	int height = 384;
	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, width, height, _bg_image[ _state ] );
	Drawer::drawGraph( 0, 0, 50, 200, 50 + 128, 200 + 128, 64, 64, _taiko_image );
}

void ScenePlay::drawBarLine( ) const {
	//	Drawer::drawLine( x, 200, x, 300 );
}

void ScenePlay::drawBullet( ) const {
	std::list< BulletPtr >::const_iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		(*ite)->draw( _taiko_image );
		ite++;
	}
}


void ScenePlay::drawTitle( ) const {
	int x = WINDOW_WIDTH - _title.length( ) * ( FONT_SIZE / 2 ) - 20;
	int y = 340;
	Drawer::drawString( x, y, _title.c_str( ) );
}

void ScenePlay::createBullet( ) {
	int idx = 0;
	int code_list_size = _code_list.size( );
	for ( int i = 0; i < code_list_size; i++ ) {
		int code_size = _code_list[ i ].size( );
		for ( int j = 0; j < code_size; j++ ) {
			int size = MAX_CODE / code_size;
			int type = _code_list[ i ][ j ];
			int idx = ( MAX_CODE * i ) + ( j * size );
			if ( type > 4 || type == 0 ) {
				continue;
			}
			_bullets.push_back( BulletPtr( new Bullet( (Bullet::TYPE)type, ( idx ) * RATIO ) ) );
		}
	}
}