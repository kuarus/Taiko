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
	int addNum( int num1, int num2 );
private:
	Game::RESULT _result;
	Game::RESULT _view;
	int _count;
	int _bg_image;
};

