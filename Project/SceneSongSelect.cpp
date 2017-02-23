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
_music( 0 ) {
	_bg_image = Drawer::loadGraph( "Resource/img/song_select_bg.png" );
	_songs = songs;
	audition( );
	Drawer::changeFont( V_FONT );
}


SceneSongSelect::~SceneSongSelect( ) {
	Drawer::deleteGraph( _bg_image );
	Sound::stopSound( _music );
	Sound::deleteSound( _music );
}

void SceneSongSelect::update( GamePtr game ) {
	select( game );
	if ( game->isNext( ) ) {
		game->setSelectSong( _select );
		game->setScene( Game::SCENE::SCENE_PLAY );
	}
	if ( game->isBack( ) ) {
		game->setScene( Game::SCENE::SCENE_MENU );
	}
}

void SceneSongSelect::draw( ) {
	Drawer::drawGraph( 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 512, 384, _bg_image );
	drawSongList( );
}

void SceneSongSelect::drawSelecting( unsigned int color ) {
	std::string title =  _songs->getTitle( _select );
	int level = _songs->getLevel( _select );
	int x = SELECTING_X;
	int y = MENU_Y;
	drawSong( x, y, x + SELECTING_WIDTH, y + MENU_HEIGHT, color, _select );
	Drawer::drawVString( x + FONT_SIZE + 30 + 10, y + 40, "“ïˆÕ“x" );
	Drawer::drawVString( x + 30, y + 20, "<‹S>" );

	for ( int j = 1; j <= MAX_LEVEL; j++ ) {
		std::string star = "™";
		if ( MAX_LEVEL - j <= level ) {
			star = "š";
		}
		Drawer::drawVString( x + 30, y + 60 + j * FONT_SIZE, star.c_str( ) );
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
		int idx1 = ( _select - i - 1 + list_size ) % list_size;
		int x0 = SELECTING_X + SELECTING_WIDTH + INTERVAL + i * ( menu_width + INTERVAL );
		int x1 = SELECTING_X - INTERVAL - menu_width - i * ( menu_width + INTERVAL );
		drawSong( x0, MENU_Y, x0 + menu_width, MENU_HEIGHT, menu_color, idx0 );
		drawSong( x1, MENU_Y, x1 + menu_width, MENU_HEIGHT, menu_color, idx1 );
	}
}

void SceneSongSelect::select( GamePtr game ) {
	bool push = false;
	if ( game->isLeft( ) ) {
		_select--;
		push = true;
	}
	if ( game->isRight( ) ) {
		_select++;
		push = true;
	}
	std::vector< Songs::SONG > song_list = _songs->getSongList( );
	_select %= song_list.size( );
	if ( _select < 0 ) {
		_select += song_list.size( );
	}

	if ( push ) {
		audition( );
	}
}

void SceneSongSelect::audition( ) {
	Sound::stopSound( _music );
	Songs::SONG music_data = _songs->getSongData( _select );
	std::string music_file = music_data.directory + "/" + _songs->getMusicFileName( _select );
	_music = Sound::loadSound( music_file.c_str( ) );
	Sound::playSound( _music, true );
	Sound::changeVol( 90, _music );
}