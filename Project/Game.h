#pragma once
#include <string>
#include <array>
#include "Songs.h"
#include "define.h"
#include "smart_ptr.h"
#include "DxLib.h"

PTR( Game );
PTR( Scene );
PTR( Songs );

class Game : public std::enable_shared_from_this< Game > {
public:
	Game( );
	virtual ~Game( );
public:
	enum SCENE {
		SCENE_TITLE,
		SCENE_MENU,
		SCENE_SONG_SELECT,
		SCENE_PLAY,
		SCENE_OPTION,
		SCENE_RESULT,
		MAX_SCENE,
	};
	enum KEY {
		KEY_D = KEY_INPUT_D,
		KEY_F = KEY_INPUT_F,
		KEY_J = KEY_INPUT_J,
		KEY_K = KEY_INPUT_K,
		KEY_Q = KEY_INPUT_Q,
		KEY_SPACE = KEY_INPUT_SPACE,
		KEY_BACK = KEY_INPUT_BACK,
		KEY_LEFT = KEY_INPUT_LEFT,
		KEY_RIGHT = KEY_INPUT_RIGHT,
		KEY_ESCAPE = KEY_INPUT_ESCAPE,
		MAX_KEY
	};
	struct RESULT {
		int combo;
		int max_combo;
		int score;
		RESULT( ) {
			this->combo = 0;
			this->max_combo = 0;
			this->score = 0;
		}
	};
public:
	static std::shared_ptr< Game > getPtr( );
	static void initialize( );
public:
	void setScene( SCENE scene );
	void setSelectSong( int select );
	void run( );
	void setFinish( );
	void setResult( RESULT result );
	bool isPushKey( int key ) const;
	bool isHoldKey( int key ) const;
	bool isNext( ) const;
	bool isBack( ) const;
	bool isDongLeft( ) const;
	bool isDongRight( ) const;
	bool isKaLeft( ) const;
	bool isKaRight( ) const;
private:
	bool isFinish( );
	void update( );
	void flip( ) const;
	GamePtr getThis( );
	void changeScene( SCENE scene );
	void updateKey( );
	void updateSe( );
private:
	SCENE _scene;
	SCENE _old_scene;
	ScenePtr _scene_ptr;
	SongsPtr _songs;
	std::array< int, GRAPH_MAX > _images;
	int _selecting_song;
	int _sound_dong;
	int _sound_ka;
	RESULT _result;
	bool _is_finish;
	char _key[ 256 ];
};