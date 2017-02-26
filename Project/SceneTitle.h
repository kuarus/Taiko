#pragma once
#include "Scene.h"

class SceneTitle : public Scene {
public:
	SceneTitle( );
	virtual ~SceneTitle( );
public:
	void update( GamePtr game );
	void draw( GamePtr game );
private:
	int _bg_image;
};

