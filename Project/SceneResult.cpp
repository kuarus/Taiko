#include "SceneResult.h"
#include "Drawer.h"

static const int WAIT_TIME = 50;
static const int DRAW_INTERVAL_TIME = 3;

SceneResult::SceneResult( Game::RESULT result ) :
_result( result ),
_count( 0 ) {
	_bg_image = Drawer::loadGraph( "Resource/img/resultbg_clear.png" );
}


SceneResult::~SceneResult( ) {
	Drawer::deleteGraph( _bg_image );
}

void SceneResult::update( GamePtr game ) {
	if ( _count % DRAW_INTERVAL_TIME == 0 ) {
		_view.great		= addNum( _view.great	 , _result.great );
		_view.good		= addNum( _view.good	 , _result.good  );
		_view.bad		= addNum( _view.bad		 , _result.bad   );
		_view.score		= addNum( _view.score	 , _result.score );
		_view.combo		= addNum( _view.combo	 , _result.combo );
		_view.max_combo = addNum( _view.max_combo, _result.max_combo );
	}

	if ( _count > WAIT_TIME ) {
		if ( game->isNext( ) || game->isBack( ) ) {
			game->setScene( Game::SCENE::SCENE_SONG_SELECT );
		}
	}
	_count++;
}

void SceneResult::draw( GamePtr game ) {
	unsigned int color = Drawer::getColor( 255, 255, 255 );
	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 512, 384, _bg_image );
	SetFontSize( FONT_SIZE * 3 );
	Drawer::drawString( 280, 60, color, "リザルト" );
	SetFontSize( FONT_SIZE );
	int x = 300;
	int y = 200;
	Drawer::drawString( x, y, color, "　良：%d", _view.great );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "　可：%d", _view.good );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "不可：%d", _view.bad );

	x = 500;
	y = 200;
	Drawer::drawString( x, y, color, "　　スコア：%d", _view.score );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "最終コンボ：%d", _view.combo );
	y += FONT_SIZE;
	Drawer::drawString( x, y, color, "最大コンボ：%d", _view.max_combo );

	Drawer::drawString( 100, 0, color, "曲選択に戻る:<Q><BackSpace><Space><D><F><J><K>" );
}

int SceneResult::addNum( int num1, int num2 ) {
	int result = num1;
	if ( num1 != num2 ) {
		int tmp = num2;
		int digit = 0;
	
		while ( tmp != 0 ) {
			tmp /= 10;
			digit++;
		}
		if ( digit < 2 ) {
			if ( result < num2 ) {
				result++;
			}
		}
	
		for ( int i = digit - 2; i >= 0; i-- ) {
			int num = 1;
			for ( int j = 0; j < i; j++ ) {
				num *= 11;
			}
			if ( result <= num2 - num ) {
				result += num;
			}
		}
	}
	return result;
}