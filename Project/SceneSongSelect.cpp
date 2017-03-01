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
_music( 0 ),
_state( STATE::STATE_SELECT_SONG ),
_selecting_diff( Songs::DIFF::EASY ) {
	_bg_image = Drawer::loadGraph( "Resource/img/song_select_bg.png" );
	_songs = songs;
	audition( );
	Drawer::changeFont( V_FONT );
}


SceneSongSelect::~SceneSongSelect( ) {
	Drawer::deleteGraph( _bg_image );
	Sound::stop( _music );
	Sound::destroy( _music );
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
			game->setSelectSong( _select, _selecting_diff );
			game->setScene( Game::SCENE::SCENE_PLAY );
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

void SceneSongSelect::drawSelecting( unsigned int color ) {
	std::string title =  _songs->getTitle( _select );
	int x = SELECTING_X;
	int y = MENU_Y;
	drawSong( x, y, x + SELECTING_WIDTH, y + MENU_HEIGHT, color, _select );
	y += 30;
	x += 10;
	for ( int i = 0; i < Songs::DIFF::MAX_DIFF; i++ ) {
		bool selecting = false;
		if ( _state == STATE::STATE_SELECT_DIFF ) {
			if ( i == (int)_selecting_diff ) {
				selecting = true;
			}
		}
		Songs::DIFF diff = (Songs::DIFF)i;
		int level = _songs->getLevel( _select, diff );
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
		Drawer::drawVString( x, y + 20, diff_str.c_str( ), selecting );
		for ( int j = 1 + i; j <= MAX_LEVEL; j++ ) {
			std::string star = "☆";
			if ( MAX_LEVEL - j <= level ) {
				star = "★";
			}
			Drawer::drawVString( x, y + FONT_SIZE * 5 + j * FONT_SIZE, star.c_str( ) );
		}
		x += FONT_SIZE + 20;
	}	
}

void SceneSongSelect::drawSong( int x1, int y1, int x2, int y2, unsigned int color, int idx ) {
	std::string title =  _songs->getTitle( idx );

	Drawer::drawBox( x1, y1, x2, y2, color );
	Drawer::drawVString( x2 - 60, y1 + 20, title.c_str( ) );
}

void SceneSongSelect::drawSongList( ) {
	std::vector< Songs::SONG > song_list = _songs->getSongList( );
	int list_size = song_list.size( );
	int menu_width = 150;
	int sx1 = SELECTING_X;
	int sx2 = sx1 + menu_width;
	int sy1 = MENU_Y;
	int sy2 = sy1 + MENU_HEIGHT;
	int menu_color = Drawer::getColor( 50, 255, 50 );
	drawSelecting( menu_color );

	for ( int i = 0; i < VIEW_NUM; i++ ) {
		int idx0 = ( _select + i + 1 ) % list_size;
		int idx1 = ( _select - i - 1 ) % list_size;
		if ( idx1 < 0 ) {
			idx1 += list_size;
		}
		int x0 = SELECTING_X + SELECTING_WIDTH + INTERVAL + i * ( menu_width + INTERVAL );
		int x1 = SELECTING_X - INTERVAL - menu_width - i * ( menu_width + INTERVAL );
		drawSong( x0, MENU_Y, x0 + menu_width, MENU_HEIGHT, menu_color, idx0 );
		drawSong( x1, MENU_Y, x1 + menu_width, MENU_HEIGHT, menu_color, idx1 );
	}
}

void SceneSongSelect::select( GamePtr game ) {
	std::vector< Songs::SONG > song_list = _songs->getSongList( );
	int list_size = song_list.size( );
	bool push = false;
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
			audition( );
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

void SceneSongSelect::audition( ) {
	Sound::stop( _music );
	Songs::SONG music_data = _songs->getSongData( _select );
	std::string music_file = music_data.directory + "/" + _songs->getMusicFileName( _select );
	_music = Sound::load( music_file.c_str( ) );
	Sound::playMusic( _music, true );
	Sound::changeVol( 210, _music );
}