#include "Bullet.h"
#include "Drawer.h"
#include "Game.h"

static const int POSITION_Y = 200;
static const int CHIP_SIZE = 64;
static const int MOVE_SPEED = 30;
static const int INIT_SPEED_Y = -20;
static const int JUDGE_X = 35;
static const int JUDGE_GOOD_RANGE = 20;

Bullet::Bullet( TYPE type, int idx ) {
	_type = type;
	_idx = idx;
	_turn = false;
	_x = 2000;
	_count = 0;
	_y = POSITION_Y;
	_speed = INIT_SPEED_Y;
	_width = 128;
	_height = 128;
	_chip_pos[ TYPE::TYPE_DONG		] = CHIP_POS( 1, 0 );
	_chip_pos[ TYPE::TYPE_KA		] = CHIP_POS( 2, 0 );
	_chip_pos[ TYPE::TYPE_BIG_DONG	] = CHIP_POS( 3, 0 );
	_chip_pos[ TYPE::TYPE_BIG_KA	] = CHIP_POS( 4, 0 );
}


Bullet::~Bullet( ) {
}

void Bullet::update( int idx, GamePtr game ) {
	if ( !_turn ) {
		_x = ( _idx - idx ) + JUDGE_X;
	}
	if ( _turn ) {
		_x += MOVE_SPEED;
		_y += _speed;
		_speed++;
		if ( _x > 800 ) {
			_x = - 200;
		}
	}
	if ( checkJudge( idx, game ) != JUDGE::JUDGE_NONE ) {
		_turn = true;
	}
}

void Bullet::draw( int image ) const {
	int tx = _chip_pos[ _type ].tx;
	int ty = _chip_pos[ _type ].ty;
	Drawer::drawGraph( tx, ty, _x, _y, _x + _width, _y + _height, CHIP_SIZE, CHIP_SIZE, image );
}

Bullet::JUDGE Bullet::checkJudge( int idx, GamePtr game ) {
	Bullet::JUDGE result = JUDGE::JUDGE_NONE;
	bool push = false;
	switch ( _type ) {
	case TYPE::TYPE_DONG:
		if ( game->isDongLeft( ) || game->isDongRight( ) ) {
			push = true;
		}
		break;
	case TYPE::TYPE_KA:
		if ( game->isKaLeft( ) || game->isKaRight( ) ) {
			push = true;
		}
		break;
	case TYPE::TYPE_BIG_DONG:
		if ( game->isDongLeft( ) || game->isDongRight( ) ) {
			push = true;
		}
		break;
	case TYPE::TYPE_BIG_KA:
		if ( game->isKaLeft( ) || game->isKaRight( ) ) {
			push = true;
		}
		break;
	}
	if ( push ) {
		int judge = _idx - idx;
		if ( judge >= - JUDGE_GOOD_RANGE && judge <= JUDGE_GOOD_RANGE ) {
			//‘¾ŒÛ‚ð‚½‚½‚¢‚½Žž‚Ì”»’è–¢ŽÀ‘•
			result = JUDGE::JUDGE_GOOD;
		}
		
	}
	return result;
}

bool Bullet::isOutSideScreen( ) {
	return _x < -_width;
}