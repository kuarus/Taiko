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
	enum STATE {
		STATE_SELECT_SONG,
		STATE_SELECT_DIFF,
		MAX_STATE,
	};
private:
	void drawSelecting( );
	void drawSong( int x1, int y1, int x2, int y2, int idx );
	void drawSongList( );
	void select( GamePtr game );
	void audition( int old_select );
private:
	int _bg_image;
	int _select;
	int _music[ 1024 ];
	STATE _state;
	Songs::DIFF _selecting_diff;
	std::vector< Songs::SONG_INFO > _song_list;
};

