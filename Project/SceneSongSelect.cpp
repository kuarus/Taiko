#include "SceneSongSelect.h"
#include "Drawer.h"
#include "Sound.h"
#include "Game.h"

static const int MAX_LEVEL = 10;
static const int VIEW_NUM = 10;
static const int SELECTING_WIDTH = 300;
static const int SELECTING_X = 400;
static const int MENU_HEIGHT = 550;
static const int MENU_Y = 30;
static const int INTERVAL = 20;

SceneSongSelect::SceneSongSelect( SongsPtr songs, int select ) :
_select( select ),
_state( STATE::STATE_SELECT_SONG ),
_selecting_diff( Songs::DIFF::EASY ) {
	_song_list = songs->getSongInfoList( );
	_bg_image = Drawer::loadGraph( "Resource/img/song_select_bg.png" );
	Drawer::changeFontSize( FONT_SIZE * 4 );
	for ( int i = 0; i < 1024; i++ ) {
		if ( i >= (int)_song_list.size( ) ) {
			_music[ i ] = 0;
			continue;
		}
		if ( i % 20 == 0 ) {
			int pattern = i / 20 % 4;
			Drawer::drawLoading( pattern );
		}
		_music[ i ] = Sound::load( _song_list[ i ].music.c_str( ) );
	}

	audition( 0 );
	Drawer::changeFontSize( FONT_SIZE );
}


SceneSongSelect::~SceneSongSelect( ) {
	Drawer::deleteGraph( _bg_image );
	Sound::stop( _music[ _select ] );
	for ( int i = 0; i < 1024; i++ ) {
		Sound::destroy( _music[ i ] );
	}
}

void SceneSongSelect::update( GamePtr game ) {
	select( game );
	switch ( _state ) {
	case STATE::STATE_SELECT_SONG:
		if ( game->isNext( ) ) {
			_state = STATE::STATE_SELECT_DIFF;
		}
		if ( game->isBack( ) ) {
			game->setScene( Game::SCENE::SCENE_MENU );
		}
		break;
	case STATE::STATE_SELECT_DIFF:
		if ( game->isNext( ) ) {
			if (  _song_list[ _select ].level[ _selecting_diff ] != 0 ) {
				game->setSelectSong( _select, _selecting_diff );
				game->setScene( Game::SCENE::SCENE_PLAY );
			}
		}
		if ( game->isBack( ) ) {
			_state = STATE::STATE_SELECT_SONG;
		}
		break;
	}
}

void SceneSongSelect::draw( GamePtr game ) {
	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 512, 384, _bg_image );
	drawSongList( );
}

void SceneSongSelect::drawSelecting( ) {
	std::string title =  _song_list[ _select ].title;
	int x = SELECTING_X;
	int y = MENU_Y;
	drawSong( x, y, x + SELECTING_WIDTH, y + MENU_HEIGHT, _select );
	y += 30;
	x += 10;
	for ( int i = 0; i < Songs::DIFF::MAX_DIFF; i++ ) {
		unsigned int color = Drawer::getColor( 255, 255, 255 );
		if ( _state == STATE::STATE_SELECT_DIFF ) {
			if ( i == (int)_selecting_diff ) {
				color = Drawer::getColor( 0, 150, 255 );
			}
		}
		Songs::DIFF diff = (Songs::DIFF)i;
		std::string diff_str;
		switch ( diff ) {
		case Songs::DIFF::ONI:
			diff_str = "おに　　　　";
			break;
		case Songs::DIFF::HARD:
			diff_str = "むずかしい　";
			break;
		case Songs::DIFF::NORMAL:
			diff_str = "ふつう　　　";
			break;
		case Songs::DIFF::EASY:
			diff_str = "かんたん　　";
			break;
		}
		Drawer::drawVString( x, y + 20, color, diff_str.c_str( ) );
		for ( int j = 1 + i; j <= MAX_LEVEL; j++ ) {
			std::string star = "☆";
			if ( MAX_LEVEL - j < _song_list[ _select ].level[ diff ] ) {
				star = "★";
			}
			Drawer::drawVString( x, y + FONT_SIZE * 5 + j * FONT_SIZE, color, star.c_str( ) );
		}
		x += FONT_SIZE + 20;
	}	
}

void SceneSongSelect::drawSong( int x1, int y1, int x2, int y2, int idx ) {
	std::string title = _song_list[ idx ].title;
	unsigned int box_color = Drawer::getColor( _song_list[ idx ].genre.color_code );
	unsigned int font_color = Drawer::getColor( _song_list[ idx ].genre.font_color );
	Drawer::drawBox( x1, y1, x2, y2, box_color );
	Drawer::drawVString( x2 - 60, y1 + 20, font_color, title.c_str( ) );
}

void SceneSongSelect::drawSongList( ) {
	int list_size = _song_list.size( );
	int menu_width = 150;
	int sx1 = SELECTING_X;
	int sx2 = sx1 + menu_width;
	int sy1 = MENU_Y;
	int sy2 = sy1 + MENU_HEIGHT;
	drawSelecting( );

	for ( int i = 0; i < VIEW_NUM; i++ ) {
		int idx0 = ( _select + i + 1 ) % list_size;
		int idx1 = ( _select - i - 1 ) % list_size;
		if ( idx1 < 0 ) {
			idx1 += list_size;
		}
		int x0 = SELECTING_X + SELECTING_WIDTH + INTERVAL + i * ( menu_width + INTERVAL );
		int x1 = SELECTING_X - INTERVAL - menu_width - i * ( menu_width + INTERVAL );
		drawSong( x0, MENU_Y, x0 + menu_width, MENU_HEIGHT, idx0 );
		drawSong( x1, MENU_Y, x1 + menu_width, MENU_HEIGHT, idx1 );
	}
}

void SceneSongSelect::select( GamePtr game ) {
	int list_size = _song_list.size( );
	bool push = false;
	int old_select = _select;
	int selecting_diff = (int)_selecting_diff;

	switch ( _state ) {
	case STATE::STATE_SELECT_SONG:
		if ( game->isKaLeft( ) ) {
			_select--;
			push = true;
		}
		if ( game->isKaRight( ) ) {
			_select++;
			push = true;
		}
		_select %= list_size;
		if ( _select < 0 ) {
			_select += list_size;
		}
		if ( push ) {
			audition( old_select );
		}
		break;
	case STATE::STATE_SELECT_DIFF:
		if ( game->isKaLeft( ) ) {
			selecting_diff--;
		}
		if ( game->isKaRight( ) ) {
			selecting_diff++;
		}
		if ( selecting_diff < 0 ) {
			selecting_diff = 0;
		}
		if ( selecting_diff >= Songs::DIFF::MAX_DIFF ) {
			selecting_diff = (int)Songs::DIFF::MAX_DIFF - 1;
		}
		_selecting_diff = (Songs::DIFF)selecting_diff;
		break;
	}
}

void SceneSongSelect::audition( int old_select ) {
	Sound::stop( _music[ old_select ] );
	//_music[ _select ] = Sound::load( _song_list[ _select ].music.c_str( ) );
	Sound::playMusic( _music[ _select ], true, _song_list[ _select ].demo_pos );
	Sound::changeVol( 240, _music[ _select ] );
}