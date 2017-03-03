#include "ScenePlay.h"
#include "Drawer.h"
#include "define.h"
#include "Songs.h"
#include "Sound.h"

static const int WAIT_APPEAR_TIME = 40;
static const int MUSIC_VOL = 80;
static const int MAX_CODE = 192;
static const int OFFSET = 0;
static const int LOAD_IDX = 1000;
static const int JUDGE_DRAW_TIME = 10;
static const int EXPLOSION_DRAW_TIME = 9;
static const int FLASH_DRAW_TIME = 5;
static const int GREAT_SCORE = 10;
static const int GOOD_SCORE = 5;
static const int VOL = 230;//MAX255

ScenePlay::ScenePlay( int select, SongsPtr songs, Songs::DIFF diff ) :
_mood( MOOD::MOOD_NORMAL ),
_play_state( PLAY_STATE::PLAY_STATE_WAIT ),
_before_seq( 0 ),
_idx( 1 ),
_result( ),
_judge( Bullet::JUDGE::JUDGE_NONE ),
_judge_draw( Bullet::JUDGE::JUDGE_NONE ),
_flash_type( Bullet::TYPE::TYPE_NONE ) {
	Drawer::changeFont( H_FONT );

	Songs::SONG_INFO song_info = songs->getInfo( select );
	std::string music_file = song_info.music;
	_title = song_info.title;
	_music = Sound::load( music_file.c_str( ) );
	_song = songs->getSongData( select, diff );
	loadImages( );
	loadSounds( );
	loadBullet( songs, select, diff );
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
		if ( game->isBack( ) ) {
			game->setScene( Game::SCENE::SCENE_SONG_SELECT );
		}
		break;
	case PLAY_STATE::PLAY_STATE_INIT:
		initialize( );
		break;
	case PLAY_STATE::PLAY_STATE_PLAY:
		updatePlay( game );
		if ( game->isBack( ) ) {
			_play_state = PLAY_STATE::PLAY_STATE_END;
		}
		break;
	case PLAY_STATE::PLAY_STATE_END:
		game->setResult( _result );
		if ( !Sound::isPlayingMusic( _combo_sound[ COMBO::COMBO_FULL ] ) ) {
			game->setScene( Game::SCENE::SCENE_RESULT );
		}
		break;
	}

}

void ScenePlay::draw( GamePtr game ) {
	drawBg( );
	drawBarLine( );
	drawFlash( game );
	drawBullet( );
	drawExplosion( );
	drawMTaiko( game );
	drawJudge( );
	drawTitle( );
	drawCombo( _result.combo );
	drawNote( game );
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
	_image[ IMAGE::IMAGE_FLASH_RED		] = Drawer::loadGraph( "Resource/img/sfieldflash_red.png" );
	_image[ IMAGE::IMAGE_FLASH_BLUE		] = Drawer::loadGraph( "Resource/img/sfieldflash_blue.png" );
}

void ScenePlay::loadSounds( ) {
	_combo_sound[ COMBO::COMBO_50   ] = Sound::load( "Resource/Sound/voice_50combo.wav" );
	_combo_sound[ COMBO::COMBO_100  ] = Sound::load( "Resource/Sound/voice_100combo.wav" );
	_combo_sound[ COMBO::COMBO_200  ] = Sound::load( "Resource/Sound/voice_200combo.wav" );
	_combo_sound[ COMBO::COMBO_300  ] = Sound::load( "Resource/Sound/voice_300combo.wav" );
	_combo_sound[ COMBO::COMBO_400  ] = Sound::load( "Resource/Sound/voice_400combo.wav" );
	_combo_sound[ COMBO::COMBO_500  ] = Sound::load( "Resource/Sound/voice_500combo.wav" );
	_combo_sound[ COMBO::COMBO_600  ] = Sound::load( "Resource/Sound/voice_600combo.wav" );
	_combo_sound[ COMBO::COMBO_700  ] = Sound::load( "Resource/Sound/voice_700combo.wav" );
	_combo_sound[ COMBO::COMBO_800  ] = Sound::load( "Resource/Sound/voice_800combo.wav" );
	_combo_sound[ COMBO::COMBO_900  ] = Sound::load( "Resource/Sound/voice_900combo.wav" );
	_combo_sound[ COMBO::COMBO_FULL ] = Sound::load( "Resource/Sound/voice_fullcombo.wav" );
}

void ScenePlay::updatePlay( GamePtr game ) {
	int now = Sound::getTime( _music );
	int seq = ( now - OFFSET + _song.offset ) * 100 / _song.pitch;
	int mark = _idx * MAX_CODE;
	if ( seq >= mark &&
		_before_seq < mark ) {
		_idx++;
	}
	_before_seq = seq;
	updateBullet( seq, game );
	updateJudge( );
	creatBullet( );
	if ( !Sound::isPlayingMusic( _music ) ) {
		if ( _result.bad == 0 ) {
			Sound::playSE( _combo_sound[ COMBO::COMBO_FULL ], false );
		}
		_play_state = PLAY_STATE::PLAY_STATE_END;
	}
}

void ScenePlay::updateJudge( ) {
	if ( _judge != Bullet::JUDGE::JUDGE_NONE ) {
		switch ( _judge ) {
		case Bullet::JUDGE::JUDGE_GREAT:
			_result.great++;
			_result.combo++;
			break;
		case Bullet::JUDGE::JUDGE_GOOD:
			_result.good++;
			_result.combo++;
			break;
		case Bullet::JUDGE::JUDGE_BAD:
			_result.bad++;
			_result.combo = 0;
			break;
		case Bullet::JUDGE::JUDGE_THROUGH:
			_result.bad++;
			_result.combo = 0;
			break;
		}
		addScore( );
		playComboSound( );
		if ( _result.max_combo < _result.combo ) {
			_result.max_combo = _result.combo;
		}
	}

	if ( _judge_draw != Bullet::JUDGE::JUDGE_NONE ) {
		if ( _judge_count > JUDGE_DRAW_TIME ) {
			_judge_draw = Bullet::JUDGE::JUDGE_NONE;
			_judge_count = 0;
		}
		_judge_count++;
	}
	_judge = Bullet::JUDGE::JUDGE_NONE;
}

void ScenePlay::updateBullet( int idx, GamePtr game ) {
	std::list< BulletPtr >::const_iterator ite = _bullets.begin( );
	std::list< BulletPtr > hits;
	BulletPtr through_bullet = BulletPtr( );

	while ( ite != _bullets.end( ) ) {
		BulletPtr bullet = *ite;
		if ( !bullet ) {
			ite++;
			continue;
		}
		bullet->update( idx, game );
		if ( !bullet->isTurn( ) ) {
			Bullet::JUDGE judge = bullet->checkJudge( idx, game );
			if ( judge != Bullet::JUDGE::JUDGE_NONE ) {
				if ( judge == Bullet::JUDGE::JUDGE_THROUGH ) {
					through_bullet = bullet;
				}
				if ( judge != Bullet::JUDGE::JUDGE_THROUGH ) {
					hits.push_back( bullet );
				}
			}
		}
		if ( bullet->isOutSideScreen( ) ) {
			ite = _bullets.erase( ite );
			continue;
		}
		ite++;
	}
	
	BulletPtr hit = BulletPtr( );
	if ( hits.size( ) != 0 ) {
		std::list< BulletPtr >::iterator hits_ite = hits.begin( );
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
	if ( !hit ) {
		if ( through_bullet ) {
			ite = _bullets.begin( );
			while ( ite != _bullets.end( ) ) {
				BulletPtr bullet = *ite;
				if ( bullet == through_bullet ) {
					setJudge( Bullet::JUDGE::JUDGE_THROUGH );
					ite = _bullets.erase( ite );
					break;
				}
				ite++;
			}
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

	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bg_width, bg_height, _image[ _mood ] );
	Drawer::drawGraph( 0, 0, judge_x1, ROAD_Y + 20, judge_x2, ROAD_Y + judge_height + 20, nots_chip_size, nots_chip_size, _image[ IMAGE::IMAGE_NOTS ] );
}

void ScenePlay::drawBarLine( ) const {
	for ( int i = 0; i < 6; i++ ) {
		int mark = ( _idx - 1 + i ) * MAX_CODE;
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
	int x = WINDOW_WIDTH - _title.length( ) * ( FONT_SIZE / 2 ) - FONT_SIZE;
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

void ScenePlay::drawJudge( ) const {
	if ( _judge_draw != Bullet::JUDGE::JUDGE_NONE &&
		 _judge_draw != Bullet::JUDGE::JUDGE_THROUGH ) {
		int chip_size = 50;
		int tx = 0;
		int ty = 0;
		int sx1 = 170;
		int sy1 = 100 - _judge_count;
		int sx2 = sx1 + chip_size * 2;
		int sy2 = sy1 + chip_size * 2;
		switch ( _judge_draw ) {
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
	}
}

void ScenePlay::drawExplosion( ) {
	if ( _judge_draw == Bullet::JUDGE::JUDGE_GREAT ||
		 _judge_draw == Bullet::JUDGE::JUDGE_GOOD ) {
		const int CHIP_SIZE = 128;
		int tx = _judge_count / ( EXPLOSION_DRAW_TIME / 9 ) + 9;
		int ty = 2 ;
		int sx1 = 150;
		int sy1 = 190;
		int sx2 = sx1 + CHIP_SIZE + 20;
		int sy2 = sy1 + CHIP_SIZE + 20;
		Drawer::drawGraph( tx, ty, sx1, sy1, sx2, sy2, CHIP_SIZE, CHIP_SIZE, _image[ IMAGE::IMAGE_EXPLOSION ] );
	}
}

void ScenePlay::drawFlash( GamePtr game ) {
	int image = 0;
	if ( game->isDongLeft( ) || game->isDongRight( ) ) {
		_flash_type = Bullet::TYPE::TYPE_DONG;
		_flash_count = 0;
	}
	if ( game->isKaLeft( ) || game->isKaRight( ) ) {
		_flash_type = Bullet::TYPE::TYPE_KA;
		_flash_count = 0;
	}
	if ( _flash_type == Bullet::TYPE::TYPE_DONG ) {
		image = _image[ IMAGE::IMAGE_FLASH_RED ];
	}
	if ( _flash_type == Bullet::TYPE::TYPE_KA ) {
		image = _image[ IMAGE::IMAGE_FLASH_BLUE ];
	}
	if ( image != 0 ) {
		int tx = 0;
		int ty = 0;
		int sx1 = JUDGE_X - 100;
		int sy1 = 180;
		int sx2 = WINDOW_WIDTH + 100;
		int sy2 = sy1 + 150;
		Drawer::drawGraph( tx, ty, sx1, sy1, sx2, sy2, 512, 65, _image[ IMAGE::IMAGE_FLASH_RED ], 200 - _flash_count * 20 );
		if ( _flash_count > FLASH_DRAW_TIME ) {
			_flash_type = Bullet::TYPE::TYPE_NONE;
		}
	}
	_flash_count++;
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

void ScenePlay::drawNote( GamePtr game ) const {
	if ( _play_state == PLAY_STATE::PLAY_STATE_WAIT ) {
		Drawer::drawString( 350, 240, "Pleas push SPACE key  start" );
	}
	if ( game->isAutomatic( ) ) {
		Drawer::drawString( 0, 0, "オート中" );
	}

	Drawer::drawString( 800, 10, "SCORE:%d", _result.score );

	int y = 400;
	Drawer::drawBox( 350, y - 10, 1000, y + FONT_SIZE * 5, Drawer::getColor( 200, 55, 55 ) );
	Drawer::drawString( 400, y, "ドン:<F><J>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, "カッ:<D><K>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, "終了:<Q><BackSpace><Escape>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, "オート:<1>" );


#if _DEBUG
	Drawer::drawString( 0, FONT_SIZE * 2, "%d", _result.great );
	Drawer::drawString( 0, FONT_SIZE * 3, "%d", _result.good );
	Drawer::drawString( 0, FONT_SIZE * 4, "%d", _result.bad );


#endif
}

void ScenePlay::loadBullet( SongsPtr songs, int select, Songs::DIFF diff ) {
	int code_list_size = _song.codes.size( );
	for ( int i = 0; i < code_list_size; i++ ) {
		int code_size = _song.codes[ i ].size( );
		int size = 0;
		if ( code_size != 0 ) {
			size = MAX_CODE / code_size;
		}
		for ( int j = 0; j < code_size; j++ ) {
			char type = _song.codes[ i ][ j ];
			if ( type > 4 || type == 0 ) {
				continue;
			}
			int idx = ( MAX_CODE * i ) + ( j * size );
			Bullet::CODE code = Bullet::CODE( );
			code.idx = idx;
			code.type = (Bullet::TYPE)type;
			_codes.push_back( code );
		}
	}
}

void ScenePlay::setJudge( Bullet::JUDGE judge ) {
	_judge = judge;
	_judge_draw = judge;
}
void ScenePlay::creatBullet( ) {
	std::vector< Bullet::CODE >::const_iterator ite = _codes.begin( );
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
		_result.score += GREAT_SCORE * ( _result.combo + 10 ) / 10;
		break;
	case Bullet::JUDGE::JUDGE_GOOD:
		_result.score += GOOD_SCORE * ( _result.combo + 10 ) / 10;
		break;
	}
}