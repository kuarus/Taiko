#include "BulletYellow.h"
#include "Drawer.h"

static const int CHIP_SIZE = 64;

BulletYellow::BulletYellow( CODE code, int length ) :
Bullet( code ),
_length( length ) {
}


BulletYellow::~BulletYellow( ) {
}


void BulletYellow::draw( int image ) const {
	int tx = 5;
	int ty = 0;
	int width = getWidth( );
	int height = getHeight( );
	int x1 = getX( ) - width / 2;
	int y1 = getY( );
	int x2 = x1 + width;
	int y2 = y1 + height;
	Drawer::drawGraph( 6, 0, x1 + width / 2, y1, x1 + width / 2 + _length, y2, CHIP_SIZE, CHIP_SIZE, image );
	Drawer::drawGraph( 7, 0, x1 + _length, y1, x1 + width + _length, y2, CHIP_SIZE, CHIP_SIZE, image );
	Drawer::drawGraph( tx, ty, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, image );
}

Bullet::JUDGE BulletYellow::checkJudge( int idx, GamePtr game ) const {
	return Bullet::JUDGE::JUDGE_NONE;
}

void BulletYellow::setTurn( ) {
}

bool BulletYellow::isOutSideScreen( ) const {
	return ( getIdx( ) + _length ) < -128;
}
