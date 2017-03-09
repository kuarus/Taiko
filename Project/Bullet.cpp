#include "Bullet.h"
#include "Drawer.h"
#include "Game.h"

static const int POSITION_Y = 200;
static const int CHIP_SIZE = 64;

Bullet::Bullet( CODE code ) {
	_code = code;
	_code.num /= 2;
	_finish = false;
	_x = 2000;
	_count = 0;
	_y = POSITION_Y;
	_width = 128;
	_height = 128;
	_through = false;
}


Bullet::~Bullet( ) {
}

bool Bullet::isOutSideScreen( ) const {
	return _x < -_width;
}

int Bullet::getIdx( ) const {
	return _code.idx;
}

int Bullet::getDistanceToJudge( ) const {
	return abs( _code.idx - _seq_idx );
}

void Bullet::setThroughed( ) {
	_through = true;
}

void Bullet::setFinished( ) {
	_finish = true;
}

bool Bullet::isFinished( ) const {
	return _finish;
}

Bullet::TYPE Bullet::getType( ) const {
	return _code.type;
}

int Bullet::getX( ) const {
	return _x;
}

int Bullet::getY( ) const {
	return _y;
}

int Bullet::getWidth( ) const {
	return _width;
}

int Bullet::getHeight( ) const {
	return _height;
}

Bullet::CODE Bullet::getCode( ) const {
	return _code;
}

void Bullet::setX( int x ) {
	_x = x;
}

void Bullet::setSeqIdx( int idx ) {
	_seq_idx = idx;
}

int Bullet::getSeqIdx( ) const {
	return _seq_idx;
}

bool Bullet::isThroughed( ) const {
	return _through;
}
