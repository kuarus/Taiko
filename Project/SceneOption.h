#pragma once
#include "Scene.h"

class SceneOption : public Scene {
public:
	SceneOption( );
	virtual ~SceneOption( );
public:
	void update( GamePtr game );
	void draw( GamePtr game );
};

