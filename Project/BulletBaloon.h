#pragma once
#include "Bullet.h"
class BulletBaloon : public Bullet {
public:
	BulletBaloon( CODE code );
	virtual ~BulletBaloon( );
public:
	void update( int seq, GamePtr game );
	void draw( int* image ) const;
	JUDGE checkJudge( int idx, GamePtr game ) const;
	void setTurn( );
private:
	enum STATE {
		STATE_NORMAL,
		STATE_WAIT,
		STATE_OUT,
		MAX_STATE
	};
private:
	STATE _state;
};