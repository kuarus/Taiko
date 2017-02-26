#include "ScenePlay.h"
#include "Drawer.h"
#include "define.h"
#include "Songs.h"
#include "Sound.h"
#include "Bullet.h"


static const int WAIT_APPEAR_TIME = 40;
static const int MUSIC_VOL = 80;
static const int MAX_CODE = 128;
static const int OFFSET = 180;
static const double PITCH = 13.30;
int a = 0;


ScenePlay::ScenePlay( int select, SongsPtr songs ) :
_state( STATE::STATE_NORMAL ),
_play_state( PLAY_STATE::PLAY_STATE_WAIT ),
_count( 0 ),
_before_seq( 0 ),
_idx( 1 ) {
	loadImages( );
	Songs::SONG music_data = songs->getSongData( select );
	std::string music_file = music_data.directory + "/" + songs->getMusicFileName( select );
	_music = Sound::load( music_file.c_str( ) );
	_title = songs->getTitle( select );
	_code_list = songs->getCode( select );
 	_offset = songs->getOffset( select );
	Drawer::changeFont( H_FONT );
	createBullet( );
}


ScenePlay::~ScenePlay( ) {
	for ( int i = 0; i < IMAGE::MAX_IMAGE; i++ ) {
		Drawer::deleteGraph( _image[ i ] );
	}
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

void ScenePlay::draw( GamePtr game ) {
	drawBg( );
	drawBarLine( );
	drawBullet( );
	drawTitle( );
	drawMTaiko( game );
	if ( _play_state == PLAY_STATE::PLAY_STATE_WAIT ) {
		Drawer::drawString( 350, 240, "Pleas push SPACE key  start" );
	}
	Drawer::drawString( 0, 0, "%d", a );
}

void ScenePlay::initialize( ) {
	Sound::playSound( _music, false );
	Sound::changeVol( MUSIC_VOL, _music );
	_play_state = PLAY_STATE::PLAY_STATE_PLAY;
}

void ScenePlay::loadImages( ) {
	_image[ IMAGE::IMAGE_BG_NORMAL		] = Drawer::loadGraph( "Resource/img/bg.png" );
	_image[ IMAGE::IMAGE_BG_HOT			] = Drawer::loadGraph( "Resource/img/bg_clear.png" );
	_image[ IMAGE::IMAGE_BG_CLEAR		] = Drawer::loadGraph( "Resource/img/bg_hot.png" );
	_image[ IMAGE::IMAGE_BG_MISS		] = Drawer::loadGraph( "Resource/img/bg_miss.png" );
	_image[ IMAGE::IMAGE_BG_MISS_CLEAR	] = Drawer::loadGraph( "Resource/img/bg_miss_clear.png" );
	_image[ IMAGE::IMAGE_NOTS			] = Drawer::loadGraph( "Resource/img/notes.png" );
	_image[ IMAGE::IMAGE_MTAIKO			] = Drawer::loadGraph( "Resource/img/mtaiko.png" );
	_image[ IMAGE::IMAGE_MTAIKO_DONG	] = Drawer::loadGraph( "Resource/img/mtaikoflash_red.png" );
	_image[ IMAGE::IMAGE_MTAIKO_KA		] = Drawer::loadGraph( "Resource/img/mtaikoflash_blue.png" );
}

void ScenePlay::updatePlay( GamePtr game ) {
	int now = Sound::getTime( _music );
	int seq = (int)( (double)( now - OFFSET - _offset / 10 ) / PITCH );
	int mark = _idx * 128;
	if ( seq >= mark &&
		_before_seq < mark ) {
		_idx++;
	}
	_before_seq = seq;
	a = seq;
	updateBullet( seq, game );

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
	int bg_width = 512;
	int bg_height = 384;
	int nots_chip_size = 64;
	int judge_width = nots_chip_size * 2;
	int judge_height = nots_chip_size * 2;
	const int ROAD_Y = 180;
	const int ROAD_HEIGHT = 150;
	int judge_x1 = JUDGE_X - judge_width / 2;
	int judge_x2 = JUDGE_X + judge_width - judge_width / 2;

	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bg_width, bg_height, _image[ _state ] );
	Drawer::drawGraph( 0, 0, judge_x1, ROAD_Y + 20, judge_x2, ROAD_Y + judge_height + 20, nots_chip_size, nots_chip_size, _image[ IMAGE::IMAGE_NOTS ] );
}

void ScenePlay::drawBarLine( ) const {
	for ( int i = 0; i < 6; i++ ) {
		int mark = ( _idx - 1 + i ) * 128;
		if ( mark < 200 ) {
			continue;
		}
		int x = ( mark - _before_seq ) * SPEED + JUDGE_X;
		if ( i == 1 ) {
			Drawer::drawString( 0, 80, "%d", x );
		}
		Drawer::drawLine( x, 180, x, 330 );
	}
}

void ScenePlay::drawBullet( ) const {
	std::list< BulletPtr >::const_iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		(*ite)->draw( _image[ IMAGE::IMAGE_NOTS ] );
		ite++;
	}
}


void ScenePlay::drawTitle( ) const {
	int x = WINDOW_WIDTH - _title.length( ) * ( FONT_SIZE / 2 ) - 20;
	int y = 340;
	Drawer::drawString( x, y, _title.c_str( ) );
}

void ScenePlay::drawMTaiko( GamePtr game ) const {
	const int ROAD_Y = 180;
	int y1 = ROAD_Y + 42;
	int y2 = y1 + 55;
	Drawer::drawGraph( 0, 0, 0, -135, 450, 335, 512, 512, _image[ IMAGE::IMAGE_MTAIKO ] );
	if ( game->isDongLeft( ) ) {
		Drawer::drawGraph( 0, 0, 60 - 32, y1, 115 - 32, y2, 32, 32, _image[ IMAGE::IMAGE_MTAIKO_DONG ] );
	}
	if ( game->isDongRight( ) ) {
		Drawer::drawGraph( 1, 0, 50 + 32, y1, 110 + 32, y2, 32, 32, _image[ IMAGE::IMAGE_MTAIKO_DONG ] );
	}
	if ( game->isKaLeft( ) ) {
		Drawer::drawGraph( 0, 0, 60 - 32, y1 - 28, 115 - 32, y2 + 32, 32, 64, _image[ IMAGE::IMAGE_MTAIKO_KA ] );
	}
	if ( game->isKaRight( ) ) {
		Drawer::drawGraph( 1, 0, 50 + 32, y1 - 28, 105 + 32, y2 + 32, 32, 64, _image[ IMAGE::IMAGE_MTAIKO_KA ] );
	}
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
			_bullets.push_back( BulletPtr( new Bullet( (Bullet::TYPE)type, idx ) ) );
		}
	}
}