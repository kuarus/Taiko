#include "BulletYellow.h"
#include "Drawer.h"

static const int CHIP_SIZE = 64;

BulletYellow::BulletYellow( CODE code ) :
Bullet( code ) {
	switch ( code.type ) {
	case TYPE::TYPE_YELLOW:
		_tx = 5;
		break;
	case TYPE::TYPE_BIG_YELLOW:
		_tx = 8;
		break;
	}
}


BulletYellow::~BulletYellow( ) {
}

void BulletYellow::update( int seq, GamePtr game ) {
	setSeqIdx( seq );
	CODE code = getCode( );
	int x = getX( );
	int y = getY( );
	x = (int)( (double)( code.idx - seq ) * SPEED * code.speed + JUDGE_X );
	setX( x );
}

void BulletYellow::draw( int* image ) const {
	Bullet::CODE code = getCode( );
	int width = getWidth( );
	int height = getHeight( );
	int x1 = getX( ) - width / 2;
	int y1 = getY( );
	int x2 = x1 + width;
	int y2 = y1 + height;
	//ç∂
	Drawer::drawGraph( _tx, 0, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
	//íÜ
	x1 = x2;
	x2 = (int)( x1 + code.num * SPEED * code.speed );
	Drawer::drawGraph( _tx + 1, 0, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
	//âE
	x1 = x2;
	x2 = x1 + width;
	Drawer::drawGraph( _tx + 2, 0, x1, y1, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
}

Bullet::JUDGE BulletYellow::checkJudge( GamePtr game ) const {
	return Bullet::JUDGE::JUDGE_NONE;
}

void BulletYellow::setTurn( ) {
}

bool BulletYellow::isOutSideScreen( ) const {
	Bullet::CODE code = getCode( );
	return ( code.idx + code.num * SPEED * code.speed + getWidth( ) ) < 0;
}

bool BulletYellow::checkPush( int idx, GamePtr game ) const {
	return false;
}
