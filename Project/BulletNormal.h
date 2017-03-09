#pragma once

#include "Bullet.h"

class BulletNormal : public Bullet {
public:
	BulletNormal( CODE code );
	virtual ~BulletNormal( );
public:
	void update( int seq, GamePtr game );
	void draw( int* image ) const;
	JUDGE checkJudge( GamePtr game ) const;
private:
	bool checkPush( int idx, GamePtr game ) const;
};

