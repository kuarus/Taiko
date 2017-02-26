#include "SceneTitle.h"
#include "Game.h"
#include "define.h"
#include "Drawer.h"

static const int WAIT_TIME = 20;


SceneTitle::SceneTitle( ) {
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

void SceneTitle::draw( GamePtr game ) {
	SetFontSize( FONT_SIZE * 4 );
	int x = 50;
	int y = 50;
	Drawer::drawString( x, y - 10, "太鼓のX" );
	y += FONT_SIZE * 4;
	SetFontSize( FONT_SIZE );
	Drawer::drawString( x, y, "[ドン]'F' 'J'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, "[カッ]'D' 'K'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, "[選択]'スペース' <ドン>" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, "[戻る]'Q' 'BACK' 'ESCAPE'" );
}