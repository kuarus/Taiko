#pragma once
#include "Scene.h"

class SceneResult : public Scene {
public:
	SceneResult( Game::RESULT result );
	virtual ~SceneResult( );
public:
	void update( GamePtr game );
	void draw( GamePtr game );
private:
	Game::RESULT _result;
	int _count;
	int _bg_image;
};

