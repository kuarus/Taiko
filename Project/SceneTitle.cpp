#include "SceneTitle.h"
#include "Game.h"
#include "define.h"
#include "Drawer.h"

static const int WAIT_TIME = 20;


SceneTitle::SceneTitle( ) {
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
	unsigned int color = Drawer::getColor( 255, 255, 255 );
	SetFontSize( FONT_SIZE * 4 );
	int x = 50;
	int y = 50;
	Drawer::drawString( x, y - 10, color, "���ۂ�X" );
	y += FONT_SIZE * 4;
	SetFontSize( FONT_SIZE );
	Drawer::drawString( x, y, color, "[�h��]'F' 'J'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "[�J�b]'D' 'K'" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "[�I��]'�X�y�[�X' <�h��>" );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "[�߂�]'Q' 'BACK' 'ESCAPE'" );
}