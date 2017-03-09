#include "AnimationBullet.h"
#include "define.h"
#include "Drawer.h"

static const int POSITION_Y = 200;
static const int MOVE_SPEED = 35;
static const int INIT_SPEED_Y = -38;
static const int CHIP_SIZE = 64;

AnimationBullet::AnimationBullet( Bullet::TYPE type ) {
	_x = JUDGE_X;
	_y = POSITION_Y;
	_speed_y = INIT_SPEED_Y;
	switch( type ) {
	case Bullet::TYPE::TYPE_DONG:
		_tx = 1;
		break;
	case Bullet::TYPE::TYPE_KA:
		_tx = 2;
		break;
	case Bullet::TYPE::TYPE_BIG_DONG:
		_tx = 3;
		break;
	case Bullet::TYPE::TYPE_BIG_KA:
		_tx = 4;
		break;
	default:
		break;
	};
}


AnimationBullet::~AnimationBullet( ) {
}

void AnimationBullet::update( ) {
	_x += MOVE_SPEED;
	_y += _speed_y;
	_speed_y += 3;
}

void AnimationBullet::draw( int* image ) {
	const int WIDTH = 128;
	const int HEIGHT = 128;
	int ty = 0;
	int x1 = _x - WIDTH / 2;
	int x2 = x1 + WIDTH;
	int y2 = _y + HEIGHT;

	Drawer::drawGraph( _tx, ty, x1, _y, x2, y2, CHIP_SIZE, CHIP_SIZE, *image );
}

bool AnimationBullet::isFinished( ) const {
	bool result = false;
	if ( _x > 1000 ) {
		result = true;
	}
	return result;
}