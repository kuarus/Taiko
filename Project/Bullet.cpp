#include "Bullet.h"
#include "Drawer.h"

static const int POSITION_Y = 200;
static const int START_X = 1100;
static const int CHIP_SIZE = 64;
static const int MOVE_SPEED = 10;
static const int JUDGE_X = 20;

Bullet::Bullet( TYPE type, int idx ) {
	_type = type;
	_idx = idx;
	_x = START_X;
	_speed = 10;
	_width = 128;
	_height = 128;
	_chip_pos[ TYPE::TYPE_DONG		] = CHIP_POS( 1, 0 );
	_chip_pos[ TYPE::TYPE_KA		] = CHIP_POS( 2, 0 );
	_chip_pos[ TYPE::TYPE_BIG_DONG	] = CHIP_POS( 3, 0 );
	_chip_pos[ TYPE::TYPE_BIG_KA	] = CHIP_POS( 4, 0 );
}


Bullet::~Bullet( ) {
}

void Bullet::update( int idx ) {
	_x = ( _idx - idx ) * 3 / 2 + JUDGE_X;
}

void Bullet::draw( int image ) {
	int tx = _chip_pos[ _type ].tx;
	int ty = _chip_pos[ _type ].ty;
	Drawer::drawGraph( tx, ty, _x, POSITION_Y, _x + _width, POSITION_Y + _height, CHIP_SIZE, CHIP_SIZE, image );
}

int Bullet::getX( ) {
	return _x;
}

bool Bullet::isOutSideScreen( ) {
	return _x < -_width;
}