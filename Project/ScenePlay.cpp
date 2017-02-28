#include "ScenePlay.h"
#include "Drawer.h"
#include "define.h"
#include "Songs.h"
#include "Sound.h"

static const int WAIT_APPEAR_TIME = 40;
static const int MUSIC_VOL = 80;
static const int MAX_CODE = 128;
static const int OFFSET = 0;
static const int LOAD_IDX = 1000;
static const int JUDGE_DRAW_TIME = 10;
static const int EXPLOSION_DRAW_TIME = 18;
static const int GREAT_SCORE = 10;
static const int GOOD_SCORE = 5;
static const int VOL = 230;//MAX255

ScenePlay::ScenePlay( int select, SongsPtr songs ) :
_state( STATE::STATE_NORMAL ),
_play_state( PLAY_STATE::PLAY_STATE_WAIT ),
_before_seq( 0 ),
_idx( 1 ),
_result( ),
_judge( Bullet::JUDGE::JUDGE_NONE ) {
	loadImages( );
	loadSounds( );

	Songs::SONG music_data = songs->getSongData( select );
	std::string music_file = music_data.directory + "/" + songs->getMusicFileName( select );
	_music = Sound::load( music_file.c_str( ) );
	_title = songs->getTitle( select );
 	_offset = songs->getOffset( select );
	_pitch = songs->getPitch( select );
	Drawer::changeFont( H_FONT );
	loadBullet( songs, select );
}


ScenePlay::~ScenePlay( ) {
	for ( int i = 0; i < IMAGE::MAX_IMAGE; i++ ) {
		Drawer::deleteGraph( _image[ i ] );
	}
	for ( int i = 0; i < COMBO::MAX_COMBO; i++ ) {
		Sound::destroy( _combo_sound[ i ] );
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
		game->setResult( _result );
		game->setScene( Game::SCENE::SCENE_RESULT );
		break;
	}

	if ( game->isBack( ) ) {
		_play_state = PLAY_STATE::PLAY_STATE_END;
	}
}

void ScenePlay::draw( GamePtr game ) {
	drawBg( );
	drawBarLine( );
	drawBullet( );
	drawExplosion( );
	drawMTaiko( game );
	drawJudge( );
	drawTitle( );
	drawCombo( _result.combo );
	if ( _play_state == PLAY_STATE::PLAY_STATE_WAIT ) {
		Drawer::drawString( 350, 240, "Pleas push SPACE key  start" );
	}

	Drawer::drawString( 800, 10, "SCORE:%d", _result.score );

	int y = 400;
	Drawer::drawBox( 350, y - 10, 1000, y + FONT_SIZE * 4, Drawer::getColor( 200, 55, 55 ) );
	Drawer::drawString( 400, y, "ドン:<F><J>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, "カッ:<D><K>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, "終了:<Q><BackSpace><Escape>" );
}

void ScenePlay::initialize( ) {
	Sound::playMusic( _music, false );
	Sound::changeVol( VOL, _music );
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
	_image[ IMAGE::IMAGE_COMBO_NUM		] = Drawer::loadGraph( "Resource/img/combonumber_l.png" );
	_image[ IMAGE::IMAGE_JUDGE			] = Drawer::loadGraph( "Resource/img/judgement.png" );
	_image[ IMAGE::IMAGE_EXPLOSION		] = Drawer::loadGraph( "Resource/img/explosion_upper.png" );
}

void ScenePlay::loadSounds( ) {
	_combo_sound[ COMBO::COMBO_50 ] = Sound::load( "Resource/Sound/voice_50combo.wav" );
	_combo_sound[ COMBO::COMBO_100 ] = Sound::load( "Resource/Sound/voice_100combo.wav" );
	_combo_sound[ COMBO::COMBO_200 ] = Sound::load( "Resource/Sound/voice_200combo.wav" );
	_combo_sound[ COMBO::COMBO_300 ] = Sound::load( "Resource/Sound/voice_300combo.wav" );
	_combo_sound[ COMBO::COMBO_400 ] = Sound::load( "Resource/Sound/voice_400combo.wav" );
	_combo_sound[ COMBO::COMBO_500 ] = Sound::load( "Resource/Sound/voice_500combo.wav" );
	_combo_sound[ COMBO::COMBO_600 ] = Sound::load( "Resource/Sound/voice_600combo.wav" );
	_combo_sound[ COMBO::COMBO_700 ] = Sound::load( "Resource/Sound/voice_700combo.wav" );
	_combo_sound[ COMBO::COMBO_800 ] = Sound::load( "Resource/Sound/voice_800combo.wav" );
	_combo_sound[ COMBO::COMBO_900 ] = Sound::load( "Resource/Sound/voice_900combo.wav" );
}

void ScenePlay::updatePlay( GamePtr game ) {
	int now = Sound::getTime( _music );
	int seq = ( now - OFFSET + _offset ) * 100 / _pitch;
	int mark = _idx * 128;
	if ( seq >= mark &&
		_before_seq < mark ) {
		_idx++;
	}
	_before_seq = seq;
	updateBullet( seq, game );
	creatBullet( );
	if ( !Sound::isPlayingMusic( _music ) ) {
		_play_state = PLAY_STATE::PLAY_STATE_END;
	}
}

void ScenePlay::updateBullet( int idx, GamePtr game ) {
	std::list< BulletPtr >::iterator ite = _bullets.begin( );
	std::list< BulletPtr > hits;
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
		if ( !bullet->isTurn( ) ) {
			Bullet::JUDGE judge = bullet->checkJudge( idx, game );
			if ( judge != Bullet::JUDGE::JUDGE_NONE ) {
				hits.push_back( *ite );
			}
		}
		ite++;
	}
	if ( hits.size( ) != 0 ) {
		std::list< BulletPtr >::iterator hits_ite = hits.begin( );
		BulletPtr hit = BulletPtr( );
		int diff = 100;
		while ( hits_ite != hits.end( ) ) {
			BulletPtr bullet = *hits_ite;
			int tmp = _before_seq - bullet->getIdx( );
			if ( tmp < 0 ) {
				tmp *= -1;
			}
			if ( tmp < diff ) {
				diff = tmp;
				hit = bullet;
			}
			hits_ite++;
		}
		Bullet::JUDGE judge = hit->checkJudge( idx, game );
		setJudge( judge );
		if ( judge != Bullet::JUDGE::JUDGE_BAD &&
			 judge != Bullet::JUDGE::JUDGE_THROUGH ) {
			hit->setTurn( );
		}
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

void ScenePlay::drawJudge( ) {
	if ( _judge != Bullet::JUDGE::JUDGE_NONE ) {
		int chip_size = 50;
		int tx = 0;
		int ty = 0;
		int sx1 = 170;
		int sy1 = 100 - _judge_count;
		int sx2 = sx1 + chip_size * 2;
		int sy2 = sy1 + chip_size * 2;
		switch ( _judge ) {
		case Bullet::JUDGE::JUDGE_GREAT:
			ty = 0;
			break;
		case Bullet::JUDGE::JUDGE_GOOD:
			ty = 1;
			break;
		case Bullet::JUDGE::JUDGE_BAD:
			ty = 2;
			break;
		}
		Drawer::drawGraph( tx, ty, sx1, sy1, sx2, sy2, chip_size, chip_size, _image[ IMAGE::IMAGE_JUDGE ] );
		if ( _judge_count > JUDGE_DRAW_TIME ) {
			_judge = Bullet::JUDGE::JUDGE_NONE;
		}
		_judge_count++;
	}
}

void ScenePlay::drawExplosion( ) {
	if ( _judge != Bullet::JUDGE::JUDGE_NONE && 
		 _judge != Bullet::JUDGE::JUDGE_BAD ) {
		const int CHIP_SIZE = 128;
		int tx = _judge_count / ( EXPLOSION_DRAW_TIME / 18 );
		int ty = 2 ;
		int sx1 = 160;
		int sy1 = 200;
		int sx2 = sx1 + CHIP_SIZE;
		int sy2 = sy1 + CHIP_SIZE;
		Drawer::drawGraph( tx, ty, sx1, sy1, sx2, sy2, CHIP_SIZE, CHIP_SIZE, _image[ IMAGE::IMAGE_EXPLOSION ] );
	}
}

void ScenePlay::drawCombo( int num ) const {
	int tmp = num;
	const int WIDTH = 32;
	const int HEIGHT = 64;
	int x1 = 110;
	int y1 = 165;
	int y2 = y1 + 55;
	while ( tmp != 0 ) {
		int number = tmp % 10;
		tmp /= 10;
		x1 -= 30;
		int x2 = x1 + 30;
		Drawer::drawGraph( number, 0, x1, y1, x2, y2, WIDTH, HEIGHT, _image[ IMAGE::IMAGE_COMBO_NUM ] );
	}
}

void ScenePlay::loadBullet( SongsPtr songs, int select ) {
	std::vector< std::vector< char > > code_list = songs->getCode( select );
	int code_list_size = code_list.size( );
	for ( int i = 0; i < code_list_size; i++ ) {
		int code_size = code_list[ i ].size( );
		for ( int j = 0; j < code_size; j++ ) {
			int size = MAX_CODE / code_size;
			int type = code_list[ i ][ j ];
			int idx = ( MAX_CODE * i ) + ( j * size );
			if ( type > 4 || type == 0 ) {
				continue;
			}
			Bullet::CODE code;
			code.idx = idx;
			code.type = (Bullet::TYPE)type;
			_codes.push_back( code );
		}
	}
}

void ScenePlay::setJudge( Bullet::JUDGE judge ) {
	_judge = judge;
	if ( _judge != Bullet::JUDGE::JUDGE_THROUGH ) {
		_judge_count = 0;
	}
	if ( _judge != Bullet::JUDGE::JUDGE_NONE &&
		 _judge != Bullet::JUDGE::JUDGE_THROUGH ) {
		_result.combo++;
		if ( _result.max_combo < _result.combo ) {
			_result.max_combo = _result.combo;
		}
		addScore( );
		playComboSound( );
	}
	if ( _judge == Bullet::JUDGE::JUDGE_BAD ||
		 _judge == Bullet::JUDGE::JUDGE_THROUGH ) {
		_result.combo = 0;
	}
}

void ScenePlay::creatBullet( ) {
	std::vector< Bullet::CODE >::iterator ite = _codes.begin( );
	while ( ite != _codes.end( ) ) {
		Bullet::CODE code = *ite;
		if ( code.idx < _before_seq + LOAD_IDX && code.idx >= 0 ) {
			BulletPtr bullet = BulletPtr( new Bullet( code ) );
			_bullets.push_back( bullet );
			ite = _codes.erase( ite );
			continue;
		}
		ite++;
	}
}

void ScenePlay::playComboSound( ) {
	switch ( _result.combo ) {
	case 50:
		Sound::playSE( _combo_sound[ COMBO::COMBO_50 ], false );
		break;
	case 100:
		Sound::playSE( _combo_sound[ COMBO::COMBO_100 ], false );
		break;
	case 200:
		Sound::playSE( _combo_sound[ COMBO::COMBO_200 ], false );
		break;
	case 300:
		Sound::playSE( _combo_sound[ COMBO::COMBO_300 ], false );
		break;
	case 400:
		Sound::playSE( _combo_sound[ COMBO::COMBO_400 ], false );
		break;
	case 500:
		Sound::playSE( _combo_sound[ COMBO::COMBO_500 ], false );
		break;
	case 600:
		Sound::playSE( _combo_sound[ COMBO::COMBO_600 ], false );
		break;
	case 700:
		Sound::playSE( _combo_sound[ COMBO::COMBO_700 ], false );
		break;
	case 800:
		Sound::playSE( _combo_sound[ COMBO::COMBO_800 ], false );
		break;
	case 900:
		Sound::playSE( _combo_sound[ COMBO::COMBO_900 ], false );
		break;
	}
}

void ScenePlay::addScore( ) {
	switch ( _judge ) {
		case Bullet::JUDGE::JUDGE_GREAT:
			_result.score += GREAT_SCORE;
			break;
		case Bullet::JUDGE::JUDGE_GOOD:
			_result.score += GOOD_SCORE;
			break;
	}
}