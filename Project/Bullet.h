#pragma once
#include "define.h"
#include "smart_ptr.h"

PTR( Game );

class Bullet {
public:
	enum TYPE {
		TYPE_NONE,
		TYPE_DONG,
		TYPE_KA,
		TYPE_BIG_DONG,
		TYPE_BIG_KA,
		TYPE_REPEATE,
		MAX_TYPE
	};
public:
	Bullet( TYPE type, int idx );
	virtual ~Bullet( );
public:
	void update( int idx, GamePtr game );
	void draw( int image );
	int getX( );
	bool isOutSideScreen( );
private:
	struct CHIP_POS {
		int tx;
		int ty;
		CHIP_POS( ) {
			this->tx = 0;
			this->ty = 0;
		}
		CHIP_POS( int x, int y ) {
			this->tx = x;
			this->ty = y;
		}
	};
private:
	int _x;
	int _speed;
	int _width;
	int _height;
	int _idx;
	TYPE _type;
	CHIP_POS _chip_pos[ MAX_TYPE ];
};

