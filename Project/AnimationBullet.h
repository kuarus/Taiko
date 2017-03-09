#pragma once
#include "Bullet.h"

class AnimationBullet {
public:
	AnimationBullet( Bullet::TYPE type );
	virtual ~AnimationBullet( );
public:
	void update( );
	void draw( int* image );
	bool isFinished( ) const;
private:
	int _x;
	int _y;
	int _speed_y;
	int _tx;
};

