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
	enum JUDGE {
		JUDGE_NONE,
		JUDGE_GREAT,
		JUDGE_GOOD,
		JUDGE_BAD,
		JUDGE_THROUGH,
		JUDGE_MAX
	};
	struct CODE {
		int idx;
		TYPE type;
	};
public:
	Bullet( CODE code, int num = 0 );
	virtual ~Bullet( );
public:
	void update( int idx, GamePtr game );
	void draw( int image ) const;
	bool isOutSideScreen( ) const;
	JUDGE checkJudge( int idx, GamePtr game ) const;
	int getIdx( ) const;
	void setTurn( );
	bool isTurn( ) const;
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
	int _y;
	int _speed;
	int _width;
	int _height;
	bool _turn;
	int _count;
	int _num;
	CODE _code;
	CHIP_POS _chip_pos[ MAX_TYPE ];
};

