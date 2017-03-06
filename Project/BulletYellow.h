#pragma once
#include "Bullet.h"

class BulletYellow : public Bullet {
public:
	BulletYellow( CODE code, int length );
	virtual ~BulletYellow( );
public:
	void draw( int image ) const;
	JUDGE checkJudge( int idx, GamePtr game ) const;
	void setTurn( );
	bool isOutSideScreen( ) const;
private:
	int _length;
};

