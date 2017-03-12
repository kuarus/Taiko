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
		TYPE_YELLOW,
		TYPE_BIG_YELLOW,
		TYPE_BALOON,
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
		int num;
		double speed;
		TYPE type;
		CODE( ) {
			this->idx = 0;
			this->num = 0;
			this->type = TYPE::TYPE_NONE;
		}
	};
public:
	Bullet( CODE code );
	virtual ~Bullet( );
public:
	virtual void update( int seq, GamePtr game ) = 0;
	virtual void draw( int* image ) const = 0;
	virtual bool isOutSideScreen( ) const;
	virtual JUDGE checkJudge( GamePtr game ) const = 0;
	int getIdx( ) const;
	int getDistanceToJudge( ) const;
	void setThroughed( );
	void setFinished( );
	bool isFinished( ) const;
	TYPE getType( ) const;
protected:
	int getX( ) const;
	int getY( ) const;
	int getWidth( ) const;
	int getHeight( ) const;
	CODE getCode( ) const;
	void setX( int x );
	void setSeqIdx( int idx );
	int getSeqIdx( ) const;
	virtual bool checkPush( int idx, GamePtr game ) const = 0;
	bool isThroughed( ) const;
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
	int _width;
	int _height;
	bool _finish;
	bool _through;
	int _count;
	int _seq_idx;
	CODE _code;
	CHIP_POS _chip_pos[ MAX_TYPE ];
};

