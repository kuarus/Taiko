#pragma once
#include "Bullet.h"

class BulletYellow : public Bullet {
public:
	BulletYellow( CODE code );
	virtual ~BulletYellow( );
public:
	void update( int seq, GamePtr game );
	void draw( int* image ) const;
	JUDGE checkJudge( GamePtr game ) const;
	void setTurn( );
	bool isOutSideScreen( ) const;
private:
	bool checkPush( int idx, GamePtr game ) const;
private:
	int _tx;
};

