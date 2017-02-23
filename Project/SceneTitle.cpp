#include "SceneTitle.h"
#include "Game.h"
#include "define.h"
#include "Drawer.h"

static const int WAIT_TIME = 20;


SceneTitle::SceneTitle( ) {
	_bg_image = Drawer::loadGraph( "Resource/img/title.png" );
	Drawer::changeFont( H_FONT );
}


SceneTitle::~SceneTitle( ) {
	Drawer::deleteGraph( _bg_image );
}


;void SceneTitle::update( GamePtr game ) {
	if ( game->isNext( ) ) {
		game->setScene( Game::SCENE::SCENE_MENU );
	}
}

void SceneTitle::draw( ) {
	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 512, 256, _bg_image );
	
	int x = 50;
	int y = 50;
	Drawer::drawString( x, y, "[ドン]'F' 'J'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, "[カッ]'D' 'K'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, "[選択]'ドッ' 'スペース'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, "[戻る]'Q' 'BACK' 'ESCAPE'" );
}