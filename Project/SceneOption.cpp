#include "SceneOption.h"


SceneOption::SceneOption( ) {
}

SceneOption::~SceneOption( ) {
}

void SceneOption::update( GamePtr game ) {
	if ( game->isBack( ) ) {
		game->setScene( Game::SCENE::SCENE_MENU );
	}
}

void SceneOption::draw( ) {
}