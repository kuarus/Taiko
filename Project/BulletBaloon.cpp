#include "BulletBaloon.h"
#include "Drawer.h"

static const int CHIP_SIZE = 64;

BulletBaloon::BulletBaloon( CODE code ) :
Bullet( code ) {
	_state = STATE::STATE_NORMAL;
}


BulletBaloon::~BulletBaloon( ) {
}

void BulletBaloon::update( int seq, GamePtr game ) {
	_state = STATE::STATE_NORMAL;
	CODE code = getCode( );
	int idx = code.idx - seq;
	if ( idx < 0 ) {
		if ( idx <= -code.num ) {
			idx += code.num;
			_state = STATE::STATE_OUT;
		}
		if ( idx > -code.num ) {
			_state = STATE::STATE_WAIT;
			idx = 0;
		}
	}
	int x = (int)( (double)( idx * SPEED * code.speed + JUDGE_X ) );
	setX( x );
}

void BulletBaloon::draw( int* image ) const {
	int width = getWidth( );
	int height = getHeight( );
	int x1 = getX( ) - width / 2;
	int y1 = getY( );
	int x2 = x1 + width;
	int y2 = y1 + height;
	Drawer::drawGraph( 11, 0, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
	x1 = x2;
	x2 = x1 + width;
	Drawer::drawGraph( 12, 0, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
}

Bullet::JUDGE BulletBaloon::checkJudge( GamePtr game ) const {
	return Bullet::JUDGE::JUDGE_NONE;
}

void BulletBaloon::setTurn( ) {
}

bool BulletBaloon::checkPush( int idx, GamePtr game ) const {
	return false;
}
