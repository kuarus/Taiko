#include "BulletNormal.h"
#include "Drawer.h"
#include "Game.h"

static const int CHIP_SIZE = 64;

BulletNormal::BulletNormal( CODE code ) :
Bullet( code ) {
}


BulletNormal::~BulletNormal( ) {
}

void BulletNormal::update( int seq, GamePtr game ) {
	setSeqIdx( seq );
	CODE code = getCode( );
	int x = getX( );
	int y = getY( );
	x = (int)( (double)( code.idx - seq ) * SPEED * code.speed + JUDGE_X );
	setX( x );
}


void BulletNormal::draw( int* image ) const {
	CODE code = getCode( );
	int width = getWidth( );
	int height = getHeight( );
	int tx = code.type;
	int ty = 0;
	int x1 = getX( ) - width / 2;
	int y1 = getY( );
	int x2 = x1 + width;
	int y2 = y1 + height;

	Drawer::drawGraph( tx, ty, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
}

Bullet::JUDGE BulletNormal::checkJudge( GamePtr game ) const {
	JUDGE result = JUDGE::JUDGE_NONE;
	CODE code = getCode( );
	int seq = getSeqIdx( );
	if ( checkPush( seq, game ) ) {
		int judge = code.idx - seq;
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
	if ( code.idx - seq < -JUDGE_BAD_RANGE ) {
		result = JUDGE::JUDGE_THROUGH;
	}

	if ( isThroughed( ) ) {
		result = JUDGE::JUDGE_NONE;
	}
	return result;
}

bool BulletNormal::checkPush( int idx, GamePtr game ) const {
	bool push = false;
	bool automatic_just = false;
	CODE code = getCode( );
	int diff = code.idx - idx;
	if ( diff < 0 ) {
		diff *= -1;
	}
	if ( game->isAutomatic( ) && diff <= 2 ) {
		automatic_just = true;
	}
	switch ( code.type ) {
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