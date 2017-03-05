#include "Bullet.h"
#include "Drawer.h"
#include "Game.h"

static const int POSITION_Y = 200;
static const int CHIP_SIZE = 64;
static const int MOVE_SPEED = 35;
static const int INIT_SPEED_Y = -38;
static const int JUDGE_GREAT_RANGE = 4;
static const int JUDGE_GOOD_RANGE = 9;
static const int JUDGE_BAD_RANGE = 15;
static const int JUDGE_THROUGH_RANGE = 16;

Bullet::Bullet( CODE code, int num ) {
	_num = num;
	_code = code;
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
		_x = ( _code.idx - idx ) * SPEED + JUDGE_X;
	}
	if ( _turn ) {
		_x += MOVE_SPEED;
		_y += _speed;
		_speed += 3;
		if ( _x > 1000 ) {
			_x = - 200;
		}
	}
}

void Bullet::draw( int image ) const {
	int tx = _chip_pos[ _code.type ].tx;
	int ty = _chip_pos[ _code.type ].ty;
	Drawer::drawGraph( tx, ty, _x - _width / 2, _y, _x + _width - _width / 2, _y + _height, CHIP_SIZE, CHIP_SIZE, image );
}

Bullet::JUDGE Bullet::checkJudge( int idx, GamePtr game ) const {
	Bullet::JUDGE result = JUDGE::JUDGE_NONE;
	
	if ( checkPush( idx, game ) ) {
		int judge = _code.idx - idx;
		if ( judge >= - JUDGE_BAD_RANGE && judge <= JUDGE_BAD_RANGE ) {
			result = JUDGE::JUDGE_BAD;
		}
		if ( judge >= - JUDGE_GOOD_RANGE && judge <= JUDGE_GOOD_RANGE ) {
			result = JUDGE::JUDGE_GOOD;
		}
		if ( judge >= - JUDGE_GREAT_RANGE && judge <= JUDGE_GREAT_RANGE ) {
			result = JUDGE::JUDGE_GREAT;
		}
	}
	if ( _code.idx - idx < -10 ) {
		result = JUDGE::JUDGE_THROUGH;
	}
	return result;
}

bool Bullet::isOutSideScreen( ) const {
	return _x < -_width;
}

int Bullet::getIdx( ) const {
	return _code.idx;
}

void Bullet::setTurn( ) {
	_turn = true;
}

bool Bullet::isTurn( ) const {
	return _turn;
}

bool Bullet::checkPush( int idx, GamePtr game ) const {
	bool push = false;
	bool automatic_just = false;
	int diff = _code.idx - idx;
	if ( diff < 0 ) {
		diff *= -1;
	}
	if ( game->isAutomatic( ) && diff <= 2 ) {
		automatic_just = true;
	}
	switch ( _code.type ) {
	case TYPE::TYPE_DONG:
		if ( automatic_just ) {
			game->setKey( Game::SE::SE_DONG );
		}
		if ( game->isDongLeft( ) || game->isDongRight( ) ) {
			push = true;
		}
		break;
	case TYPE::TYPE_KA:
		if ( automatic_just ) {
			game->setKey( Game::SE::SE_KA );
		}
		if ( game->isKaLeft( ) || game->isKaRight( ) ) {
			push = true;
		}
		break;
	case TYPE::TYPE_BIG_DONG:
		if ( automatic_just ) {
			game->setKey( Game::SE::SE_DONG );
		}
		if ( game->isDongLeft( ) || game->isDongRight( ) ) {
			push = true;
		}
		break;
	case TYPE::TYPE_BIG_KA:
		if ( automatic_just ) {
			game->setKey( Game::SE::SE_KA );
		}
		if ( game->isKaLeft( ) || game->isKaRight( ) ) {
			push = true;
		}
		break;
	}
	return push;
}