#include "ScenePlay.h"
#include "Drawer.h"
#include "define.h"
#include "Songs.h"
#include "Sound.h"
#include "Bullet.h"


static const int WAIT_APPEAR_TIME = 40;
static const int MUSIC_VOL = 80;
static const int MAX_CODE = 32;
static const int LOAD_IDX = 2000;

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
	_music = Sound::loadSound( music_file.c_str( ) );
	_title = songs->getTitle( select );
	_code = songs->getCode( select );
	Drawer::changeFont( H_FONT );
	_bpm = songs->getBpm( select );
	for ( int i = 0; i < LOAD_IDX; i++ ) {
		createBullet( i );
	}
}


ScenePlay::~ScenePlay( ) {
	for ( int i = 0; i < STATE::MAX_STATE; i++ ) {
		Drawer::deleteGraph( _bg_image[ i ] );
	}
	Drawer::deleteGraph( _taiko_image );
	Sound::stopSound( _music );
	Sound::deleteSound( _music );
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
		updatePlay( );
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
	drawBullet( );
	drawTitle( );
	if ( _play_state == PLAY_STATE::PLAY_STATE_WAIT ) {
		Drawer::drawString( 180, 240, "Pleas push SPACE key  start" );
	}
}

void ScenePlay::initialize( ) {
	Sound::playSound( _music, false );
	Sound::changeVol( MUSIC_VOL, _music );
	_play_state = PLAY_STATE::PLAY_STATE_PLAY;
}

void ScenePlay::updatePlay( ) {
	int idx = Sound::getSoundPosition( _music ) / _bpm / 10;
	createBullet( idx + LOAD_IDX );
	updateBullet( idx );

	_count++;
	_count %= WAIT_APPEAR_TIME;
}

void ScenePlay::updateBullet( int idx ) {
	std::list< BulletPtr >::iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		BulletPtr bullet = *ite;
		if ( !bullet ) {
			continue;
		}
		bullet->update( idx );
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

void ScenePlay::drawBarLine( ) {

}

void ScenePlay::drawBullet( ) {
	std::list< BulletPtr >::iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		(*ite)->draw( _taiko_image );
		ite++;
	}
}


void ScenePlay::drawTitle( ) {
	int x = WINDOW_WIDTH - _title.size( ) * FONT_SIZE;
	int y = 340;
	Drawer::drawString( x, y, _title.c_str( ) );
}

int ScenePlay::getCode( int idx ) {
	short a = 100;
	int size = _code.size( );
	if ( idx <= 0 || size == 0 ) {
		return 0;
	}
	if ( idx / MAX_CODE >= size ) {
		return -1;
	}
	std::vector< char > tmp = _code[ idx / MAX_CODE ];
	if ( tmp.size( ) == 0 ) {
		return 0;
	}
	int analysis = idx % MAX_CODE;
	int mag = MAX_CODE * 10 / tmp.size( );
	if ( mag <= 0 ) {
		return 0;
	}
	int code = tmp[ analysis * 10 / mag ];
	if ( _old_position == analysis * 10 / mag ) {
		return 0;
	}
	if ( code > 2 ) {
		return 0;
	}
	if ( idx == _old_idx ) {
		return 0;
	}
	_old_position = analysis * 10 / mag;
	_old_idx = idx;
	return code;
}

void ScenePlay::createBullet( int idx ) {
	int code = getCode( idx );
	if ( code == -1 ) {
		//‚¨‚í‚è
		Drawer::drawString( 0, 0, "Clear" );
	}
	if( code != Bullet::TYPE::TYPE_NONE ) {
		_bullets.push_back( BulletPtr( new Bullet( (Bullet::TYPE)code, idx ) ) );
	}
}