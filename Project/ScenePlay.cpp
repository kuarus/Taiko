#include "ScenePlay.h"
#include "Drawer.h"
#include "define.h"
#include "Songs.h"
#include "Sound.h"
#include "BulletYellow.h"
#include "BulletBaloon.h"
#include "BulletNormal.h"
#include "AnimationBullet.h"


static const int WAIT_APPEAR_TIME = 40;
static const int OFFSET = 1000;
static const int LOAD_IDX = 4000;
static const int MAX_GAUGE = 36;
//score
static const int GREAT_SCORE = 200;
static const int GOOD_SCORE = 300;
//draw
static const int FLASH_DRAW_TIME = 5;
static const int JUDGE_DRAW_TIME = 12;
static const int EXPLOSION_DRAW_TIME = 9;

ScenePlay::ScenePlay( int select, SongsPtr songs, Songs::DIFF diff ) :
_mood( MOOD::MOOD_NORMAL ),
_play_state( PLAY_STATE::PLAY_STATE_WAIT ),
_before_seq( 0 ),
_idx( 0 ),
_result( ),
_start_count( 0 ),
_judge( Bullet::JUDGE::JUDGE_NONE ),
_judge_draw( Bullet::JUDGE::JUDGE_NONE ),
_flash_type( Bullet::TYPE::TYPE_NONE ) {
	Songs::SONG_INFO song_info = songs->getInfo( select );
	std::string music_file = song_info.music;
	_song = songs->getSongData( select, diff );
	_title = song_info.title;
	_music = Sound::load( music_file.c_str( ) );
	loadBullet( songs, select, diff );
	loadImages( );
	loadSounds( );
	if ( _codes.size( ) != 0 ) {
		_start_idx = _codes.front( ).idx;
	}
	_time = 0;//(int)( (double)-OFFSET + _song.offset * 1000 );
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
		if ( _song.codes.size( ) == 0 ) {
			game->setScene( Game::SCENE::SCENE_SONG_SELECT );
		}
		if ( game->isPushKey( Game::KEY::KEY_SPACE ) ) {
			_play_state = PLAY_STATE::PLAY_STATE_START;
		}
		if ( game->isBack( ) ) {
			game->setScene( Game::SCENE::SCENE_SONG_SELECT );
		}
		break;
	case PLAY_STATE::PLAY_STATE_START:
		updateStart( game );
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
	drawMTaikoBack( );
	drawSoulGauge( );
	drawBarLine( );
	drawFlash( game );
	drawBullet( );
	drawAnimationBullet( );
 	drawExplosion( );
	drawMTaikoFront( game );
	drawJudge( );
	drawTitle( );
	drawCombo( _result.combo );
	drawScore( _result.score );
	drawNote( game );
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
	_image[ IMAGE::IMAGE_SCORE_NUM		] = Drawer::loadGraph( "Resource/img/font_m.png" );
	_image[ IMAGE::IMAGE_JUDGE			] = Drawer::loadGraph( "Resource/img/judgement.png" );
	_image[ IMAGE::IMAGE_EXPLOSION		] = Drawer::loadGraph( "Resource/img/explosion_upper.png" );
	_image[ IMAGE::IMAGE_FLASH_RED		] = Drawer::loadGraph( "Resource/img/sfieldflash_red.png" );
	_image[ IMAGE::IMAGE_FLASH_BLUE		] = Drawer::loadGraph( "Resource/img/sfieldflash_blue.png" );
	_image[ IMAGE::IMAGE_NORMAGAUGE		] = Drawer::loadGraph( "Resource/img/normagauge.png" );
	_image[ IMAGE::IMAGE_SOUL			] = Drawer::loadGraph( "Resource/img/soul.png" );
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

void ScenePlay::updateStart( GamePtr game ) {
	int time = _start_count * 100 / 6;
	_start_count++;
	int now = (int)( (double)time - OFFSET + _song.offset );
	double pitch = convertMsToPitch( 0 );
	int seq = (int)( (double)now / pitch );
	int mark = _song.bars[ _idx + 1 ].idx;
	_before_seq = seq;
	updateBullet( game, seq );
	updateJudge( );
	createBullet( );
	if ( seq >= mark &&
		_before_seq < mark ) {
		_idx++;
	}
	if ( ( time >= OFFSET )  ||
		 ( _start_idx - OFFSET >= (int)( _song.offset / pitch ) ) ) {
		Sound::playMusic( _music, false );
		_play_state = PLAY_STATE::PLAY_STATE_PLAY;
	}
}

void ScenePlay::updatePlay( GamePtr game ) {
	int current_idx = _idx + 1;
	int bar_size = _song.bars.size( );
	int now = (int)( (double)Sound::getTime( _music ) + _song.offset - _time );
	double pitch = convertMsToPitch( _idx );
	int seq = (int)( (double)now / pitch ) + _song.bars[ _idx ].idx;
	if ( current_idx < bar_size ) {
		int mark = _song.bars[ current_idx ].idx;
	if ( seq >= mark &&
		_before_seq < mark ) {
			_idx++;
			int idx_size = _song.bars.size( );
			if ( _idx >= idx_size ) {
				_idx = idx_size - 1;
			}
			_time += pitch * MAX_CODE;
		}
	}
	_before_seq = seq;

	
	updateBullet( game, seq );
	updateJudge( );
	createBullet( );
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
		}
		_judge_count++;
	}
	_judge = Bullet::JUDGE::JUDGE_NONE;
}

void ScenePlay::updateBullet( GamePtr game, int seq ) {
	std::list< BulletPtr >::iterator ite = _bullets.begin( );
	std::list< BulletPtr > hits;
	BulletPtr through_bullet = BulletPtr( );
	while ( ite != _bullets.end( ) ) {
		BulletPtr bullet = *ite;
		if ( !bullet ) {
			ite++;
			continue;
		}
		bullet->update( seq, game );
		if ( bullet->isOutSideScreen( ) || bullet->isFinished( ) ) {
			ite = _bullets.erase( ite );
			continue;
		}
		Bullet::JUDGE judge = bullet->checkJudge( game );
		if ( judge != Bullet::JUDGE::JUDGE_NONE ) {
			if ( judge == Bullet::JUDGE::JUDGE_THROUGH ) {
				through_bullet = bullet;
			}
			if ( judge != Bullet::JUDGE::JUDGE_THROUGH ) {
				hits.push_back( bullet );
			}
		}
		ite++;
	}
	int diff = 100;
	
	BulletPtr hit = BulletPtr( );
	if ( hits.size( ) != 0 ) {
		std::list< BulletPtr >::iterator hits_ite = hits.begin( );
		while ( hits_ite != hits.end( ) ) {
			BulletPtr bullet = *hits_ite;
			int distance = bullet->getDistanceToJudge( );
			if ( distance < diff ) {
				diff = distance;
				hit = bullet;
			}
			hits_ite++;
		}
		Bullet::JUDGE judge = hit->checkJudge( game );
		setJudge( judge );
		if ( judge != Bullet::JUDGE::JUDGE_BAD &&
			 judge != Bullet::JUDGE::JUDGE_THROUGH ) {
			addAnimation( hit->getType( ) );
			hit->setFinished( );
		}
	}
	if ( !hit ) {
		if ( through_bullet ) {
			through_bullet->setThroughed( );
			setJudge( Bullet::JUDGE_THROUGH );
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
		int idx = _idx + i;
		if ( idx >= (int)_song.bars.size( ) ) {
			break;
		}
		int mark = idx * MAX_CODE;
		if ( mark < _start_idx ) {
			continue;
		}
		double measure_spped = 1 + ( _song.bars[ idx ].measure / 4 - 1 );
		if ( measure_spped < 1 ) {
			measure_spped = 1.0;
		}
		int x = (int)( ( mark - _before_seq ) * SPEED * _song.bars[ idx ].scroll * measure_spped + JUDGE_X );
		Drawer::drawLine( x, 180, x, 330 );
	}
}

void ScenePlay::drawBullet( ) const {
	std::list< BulletPtr >::const_iterator ite = _bullets.begin( );
	while ( ite != _bullets.end( ) ) {
		BulletPtr bullet = (*ite);
		if ( !bullet ) {
			ite++;
			continue;
		}
		bullet->draw( &(int)_image[ IMAGE::IMAGE_NOTS ] );
		ite++;
	}
}


void ScenePlay::drawTitle( ) const {
	unsigned int color = Drawer::getColor( 255, 255, 255 );
	int x = WINDOW_WIDTH - _title.length( ) * ( FONT_SIZE / 2 ) - FONT_SIZE;
	int y = 340;
	Drawer::drawString( x, y, color, _title.c_str( ) );
}

void ScenePlay::drawMTaikoBack( ) const {
	const int ROAD_Y = 180;
	int x1 = 165;
	int x2 = 450;
	int y1 = ROAD_Y + 42;
	int y2 = y1 + 55;
	Drawer::drawGraph( 1, 0, x1, -135, x2, 335, 188, 512, _image[ IMAGE::IMAGE_MTAIKO ] );
}

void ScenePlay::drawMTaikoFront( GamePtr game ) const {
	const int ROAD_Y = 180;
	int y1 = ROAD_Y + 42;
	int y2 = y1 + 55;
	Drawer::drawGraph( 0, 0, 0, -135, 165, 335, 188, 512, _image[ IMAGE::IMAGE_MTAIKO ] );
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

void ScenePlay::drawCombo( int combo ) const {
	int tmp0 = combo;
	int tmp1 = combo;
	const int WIDTH = 32;
	const int HEIGHT = 64;
	int x1 = 85;
	int y1 = 220;
	int y2 = y1 + 55;
	int digit = 0;
	while ( tmp0 != 0 ) {
		tmp0 /= 10;
		x1 += 15;
	}
	while ( tmp1 != 0 ) {
		int number = tmp1 % 10;
		tmp1 /= 10;
		x1 -= 30;
		int x2 = x1 + 30;
		Drawer::drawGraph( number, 0, x1, y1, x2, y2, WIDTH, HEIGHT, _image[ IMAGE::IMAGE_COMBO_NUM ] );
	}
}

void ScenePlay::drawScore( int score ) const {
	int tmp = score;
	const int WIDTH = 32;
	const int HEIGHT = 64;
	int x1 = 140;
	int y1 = 170;
	int y2 = y1 + 50;
	while ( tmp != 0 ) {
		int number = tmp % 10;
		tmp /= 10;
		x1 -= 20;
		int x2 = x1 + 30;
		Drawer::drawGraph( number, 0, x1, y1, x2, y2, WIDTH, HEIGHT, _image[ IMAGE::IMAGE_SCORE_NUM ] );
	}
}

void ScenePlay::drawAnimationBullet( ) {
	std::list< AnimationBulletPtr >::iterator ite = _animations.begin( );
	while ( ite != _animations.end( ) ) {
		AnimationBulletPtr anim = *ite;
		if ( !anim ) {
			ite++;
			continue;
		}
		if ( anim->isFinished( ) ) {
			ite = _animations.erase( ite );
			continue;
		}
		anim->update( );
		anim->draw( &_image[ IMAGE::IMAGE_NOTS ] );
		ite++;
	}
}

void ScenePlay::drawSoulGauge( ) const {
	const int WIDTH = 12;
	const int HEIGHT = 32;
	int x1 = 304;
	int y1 = 124;
	int y2 = y1 + 58;
	int tx = 0;
	for ( int i = -8; i < MAX_GAUGE; i++ ) {
		int x2 = x1 + 28;
		tx = i;
		if ( i < 1 ) {
			tx = 1;
		}
		Drawer::drawGraph( tx, 0, x1, y1, x2, y2, WIDTH, HEIGHT, _image[ IMAGE::IMAGE_NORMAGAUGE ] );
		x1 += 28;
	}
	Drawer::drawBox( 960, y1 + 5, WINDOW_WIDTH, y2 - 1, Drawer::getColor( 0, 0, 0 ) );
	Drawer::drawGraph( 0, 0, 950, 90, WINDOW_WIDTH, 220, 122, 122, _image[ IMAGE::IMAGE_SOUL ] );
}

void ScenePlay::drawNote( GamePtr game ) const {
	unsigned int color = Drawer::getColor( 255, 255, 255 );
	if ( _play_state == PLAY_STATE::PLAY_STATE_WAIT ) {
		Drawer::drawString( 350, 240, color, "Pleas push SPACE key  start" );
	}
	if ( game->isAutomatic( ) ) {
		Drawer::drawString( 0, 0, color, "オート中" );
	}

	int y = 400;
	Drawer::drawBox( 350, y - 10, 1000, y + FONT_SIZE * 5, Drawer::getColor( 200, 55, 55 ) );
	Drawer::drawString( 400, y, color, "ドン:<F><J>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, color, "カッ:<D><K>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, color, "終了:<Q><BackSpace><Escape>" );
	y += FONT_SIZE;
	Drawer::drawString( 400, y, color, "オート:<1>" );
}

void ScenePlay::loadBullet( SongsPtr songs, int select, Songs::DIFF diff ) {
	int codes_size = _song.codes.size( );
	for ( int i = 0; i < codes_size; i++ ) {
		Bullet::CODE code = Bullet::CODE( );
		if ( _song.codes[ i ].type >= 5 && _song.codes[ i ].type <= 7 ) {
			code.idx = _song.codes[ i ].idx;
			code.num = _song.codes[ i + 1 ].idx - _song.codes[ i ].idx;
			double measure_spped = 1 + ( _song.codes[ i ].measure / 4 - 1 );
			if ( measure_spped < 1 ) {
				measure_spped = 1.0;
			}
			code.speed = _song.codes[ i ].scroll * measure_spped;//要調整(measure)
			code.type = (Bullet::TYPE)_song.codes[ i ].type;
			_codes.push_back( code );
			i++;
			continue;
		}
		if ( _song.codes[ i ].type == 0 ) {
			continue;
		}
		if ( _song.codes[ i ].type >= 9 ) {
			continue;
		}
		code.idx = _song.codes[ i ].idx;
		double measure_spped = 1 + ( _song.codes[ i ].measure / 4 - 1 );
			if ( measure_spped < 1 ) {
				measure_spped = 1.0;
			}
			code.speed = _song.codes[ i ].scroll * measure_spped;//要調整(measure)
		code.type = (Bullet::TYPE)_song.codes[ i ].type;
		_codes.push_back( code );
	}
}

void ScenePlay::setJudge( Bullet::JUDGE judge ) {
	_judge = judge;
	_judge_draw = judge;
	_judge_count = 0;
}

void ScenePlay::createBullet( ) {
	if ( _codes.size( ) == 0 ) {
		return;
	}
	Bullet::CODE code = _codes.front( );
	if ( code.idx < _idx * MAX_CODE + LOAD_IDX ) {
		BulletPtr bullet = BulletPtr( );
		switch ( code.type ) {
		case Bullet::TYPE::TYPE_DONG:
			bullet = BulletPtr( new BulletNormal( code ) );
			break;
		case Bullet::TYPE::TYPE_KA:
			bullet = BulletPtr( new BulletNormal( code ) );
			break;
		case Bullet::TYPE::TYPE_BIG_DONG:
			bullet = BulletPtr( new BulletNormal( code ) );
			break;
		case Bullet::TYPE::TYPE_BIG_KA:
			bullet = BulletPtr( new BulletNormal( code ) );
			break;
		case Bullet::TYPE::TYPE_YELLOW:
			bullet = BulletPtr( new BulletYellow( code ) );
			break;
		case Bullet::TYPE::TYPE_BIG_YELLOW:
			bullet = BulletPtr( new BulletYellow( code ) );
			break;
		case Bullet::TYPE::TYPE_BALOON:
			bullet = BulletPtr( new BulletBaloon( code ) );
			break;
		default:
			break;
		}
		_codes.erase( _codes.begin( ) );
		_bullets.push_back( bullet );
	}
}

void ScenePlay::playComboSound( ) {
	switch ( _result.combo ) {
	case 50:
		Sound::playSE( _combo_sound[ COMBO::COMBO_50  ], false );
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
		_result.score += (int)( (double)( GREAT_SCORE * ( _result.combo + 10 ) / 10 ) ) / 10;
		break;
	case Bullet::JUDGE::JUDGE_GOOD:
		_result.score += (int)( (double)( GOOD_SCORE  * ( _result.combo + 10 ) / 10 ) ) / 10;
		break;
	}
}

void ScenePlay::addAnimation( Bullet::TYPE type ) {
	_animations.push_back( AnimationBulletPtr( new AnimationBullet( type ) ) );
}


double ScenePlay::convertMsToPitch( int idx ) {
	if ( idx == -1 ) {
		return 0;
	}
	//ms→bpm
	//bpm = 60 / ms * measure
	//
	//bpm→ms
	//ms = 60 / bpm * measure
	double pitch = 60.0 / _song.bars[ idx ].bpm * _song.bars[ idx ].measure * 1000 / MAX_CODE;
	return pitch;
}