#pragma once
#include "Game.h"
#include "smart_ptr.h"

PTR( Game );

class Scene {
public:
	Scene( );
	virtual ~Scene( );
public:
	virtual void update( GamePtr game ) = 0;
	virtual void draw( ) { };
};

