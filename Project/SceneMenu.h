#pragma once
#include "Scene.h"

class SceneMenu : public Scene {
public:
	SceneMenu( );
	virtual ~SceneMenu( );
private:
	enum MENU {
		MENU_SONG_SELECT,
		MENU_OPTION,
		MAX_MENU
	};
public:
	void update( GamePtr game );
	void draw( GamePtr game );
	void select( GamePtr game );
private:
	int _select;
};

