#include "SceneResult.h"
#include "Drawer.h"

static const int WAIT_TIME = 50;

SceneResult::SceneResult( Game::RESULT result ) :
_result( result ),
_count( 0 ) {
	_bg_image = Drawer::loadGraph( "Resource/img/resultbg_clear.png" );
}


SceneResult::~SceneResult( ) {
	Drawer::deleteGraph( _bg_image );
}

void SceneResult::update( GamePtr game ) {
	if ( _count > WAIT_TIME ) {
		if ( game->isNext( ) || game->isBack( ) ) {
			game->setScene( Game::SCENE::SCENE_SONG_SELECT );
		}
	}
	_count++;
}

void SceneResult::draw( GamePtr game ) {
	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 512, 384, _bg_image );
	SetFontSize( FONT_SIZE * 3 );
	Drawer::drawString( 280, 60, "リザルト" );
	SetFontSize( FONT_SIZE );
	int y = 200;
	Drawer::drawString( 300, y, "　　スコア：%d", _result.score );
	y += FONT_SIZE;
	Drawer::drawString( 300, y, "最終コンボ：%d", _result.combo );
	y += FONT_SIZE;
	Drawer::drawString( 300, y, "最大コンボ：%d", _result.max_combo );

	Drawer::drawString( 100, 0, "曲選択に戻る:<Q><BackSpace><Space><D><F><J><K>" );
}
