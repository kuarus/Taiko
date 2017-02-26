#pragma once
#include "Scene.h"
#include <list>
#include <string>

PTR( Songs );
PTR( Bullet );

class ScenePlay : public Scene {
public:
	ScenePlay( int select, SongsPtr songs );
	virtual ~ScenePlay( );
public:
	void update( GamePtr game );
	void draw( GamePtr game );
private:
	enum STATE {
		STATE_NORMAL,
		STATE_CLEAR,
		STATE_HOT,
		STATE_MISS,
		STATE_MISS_CLEAR,
		MAX_STATE
	};
	enum PLAY_STATE {
		PLAY_STATE_WAIT,
		PLAY_STATE_INIT,
		PLAY_STATE_PLAY,
		PLAY_STATE_END,
		MAX_PLAY_STATE
	};
	enum IMAGE {
		IMAGE_BG_NORMAL,
		IMAGE_BG_CLEAR,
		IMAGE_BG_HOT,
		IMAGE_BG_MISS,
		IMAGE_BG_MISS_CLEAR,
		IMAGE_NOTS,
		IMAGE_MTAIKO,
		IMAGE_MTAIKO_DONG,
		IMAGE_MTAIKO_KA,
		MAX_IMAGE
	};
private:
	void initialize( );
	void loadImages( );
	void updatePlay( GamePtr game );
	void updateBullet( int idx, GamePtr game );
	void drawBg( );
	void drawBarLine( ) const;
	void drawBullet( ) const;
	void drawTitle( ) const;
	void drawMTaiko( GamePtr game ) const;
	void createBullet( );
private:
	int _image[ IMAGE::MAX_IMAGE ];
	STATE _state;
	PLAY_STATE _play_state;
	int _count;
	int _music;
	std::list< BulletPtr > _bullets;
	std::string _title;
	std::vector< std::vector< char > > _code_list;
	int _offset;
	bool _start;
	int _before_seq;
	int _dong;
	int _bpm;
	int _idx;
};

