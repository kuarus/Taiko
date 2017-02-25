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
	void draw( int image ) const;
	bool isOutSideScreen( );
private:
	enum JUDGE {
		JUDGE_NONE,
		JUDGE_GOOD,
		JUDGE_PASS,
		JUDGE_BAD,
		JUDGE_MAX
	};
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
	JUDGE checkJudge( int idx, GamePtr game );
private:
	int _x;
	int _y;
	int _speed;
	int _width;
	int _height;
	int _idx;
	bool _turn;
	int _count;
	TYPE _type;
	CHIP_POS _chip_pos[ MAX_TYPE ];
};

