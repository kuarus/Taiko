#pragma once
#include "Scene.h"

class SceneSongSelect : public Scene {
public:
	SceneSongSelect( SongsPtr songs, int select = 0 );
	virtual ~SceneSongSelect( );
public:
	void update( GamePtr game );
	void draw( GamePtr game );
private:
	void drawSelecting( unsigned int color );
	void drawSong( int x1, int y1, int x2, int y2, unsigned int color, int idx );
	void drawSongList( );
	void select( GamePtr game );
	void audition( );
private:
	int _bg_image;
	int _select;
	int _music;
	SongsPtr _songs;
};

