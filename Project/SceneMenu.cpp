#include "SceneMenu.h"
#include "Drawer.h"

static const int MENU_X = 50;
static const int MENU_Y = 50;
static const int MENU_NUM = 3;
static const int WAIT_TIME = 20;
static const unsigned int MENU_COLOR = Drawer::getColor( 255, 130, 70 );

SceneMenu::SceneMenu( ) :
_select( 0 ) {
	Drawer::changeFont( V_FONT );
}


SceneMenu::~SceneMenu( ) {
}

void SceneMenu::update( GamePtr game ) {
	select( game );
	if ( game->isNext( ) ) {
		switch( _select ) {
		case 0:
			game->setScene( Game::SCENE::SCENE_SONG_SELECT );
			break;
		case 1:
			game->setScene( Game::SCENE::SCENE_OPTION );
			break;
		case 2:
			game->setFinish( );
			break;
		}
	}
	if ( game->isBack( ) ) {
		game->setScene( Game::SCENE::SCENE_TITLE );
	}
}

void SceneMenu::draw( ) {
	int menu_width = 100;
	int menu_height = 600;
	int sx1 = MENU_X;
	int sx2 = sx1 + menu_width;
	int sy1 = MENU_Y;
	int sy2 = sy1 + menu_height;
	
	for ( int i = 0; i < MENU_NUM; i++ ) {
		bool selecting = false;
		if ( i == _select ) {
			selecting = true;
		}
		sx1 = ( WINDOW_WIDTH - 100 ) / MENU_NUM * i - menu_width / 2 + 200;
		sx2 = sx1 + menu_width;
 		Drawer::drawBox( sx1, sy1, sx2, sy2, MENU_COLOR );
		unsigned int color = Drawer::getColor( 255, 255, 255 );
		switch( i ) {
		case 0:
			Drawer::drawVString( sx1 + 30, sy1 + 50, "‹È‚ð‘I‘ð‚·‚é", selecting );
			break;
		case 1:
			Drawer::drawVString( sx1 + 30, sy1 + 50, "Ý’è", selecting );
			break;
		case 2:
			Drawer::drawVString( sx1 + 30, sy1 + 50, "ƒQ[ƒ€‚ðI—¹‚·‚é", selecting );
			break;
		}
	}
}

void SceneMenu::select( GamePtr game ) {
	if ( game->isKaLeft( ) ) {
		_select--;
	}

	if ( game->isKaRight( ) ) {
		_select++;
	}
	if ( _select > MENU_NUM - 1 ) {
		_select = MENU_NUM - 1;
	}
	if ( _select < 0 ) {
		_select = 0;
	}
}