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
	void draw( );
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
private:
	void initialize( );
	void updatePlay( GamePtr game );
	void updateBullet( int idx, GamePtr game );
	void drawBg( );
	void drawBarLine( ) const;
	void drawBullet( ) const;
	void drawTitle( ) const;
	void createBullet( );
private:
	int _bg_image[ STATE::MAX_STATE ];
	int _taiko_image;
	STATE _state;
	PLAY_STATE _play_state;
	int _count;
	int _music;
	std::list< BulletPtr > _bullets;
	std::string _title;
	std::vector< std::vector< char > > _code_list;
	int _offset;
	bool _start;
	int _bpm;
	int _old_idx;
	int _old_position;
};

