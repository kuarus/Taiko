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
		KEY_1 = KEY_INPUT_1,
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
	enum SE {
		SE_DONG,
		SE_KA,
		MAX_SE,
	};
	struct RESULT {
		int combo;
		int max_combo;
		int score;
		int great;
		int good;
		int bad;
		RESULT( ) {
			this->combo = 0;
			this->max_combo = 0;
			this->score = 0;
			this->great = 0;
			this->good = 0;
			this->bad = 0;
		}
	};
public:
	static std::shared_ptr< Game > getPtr( );
	static void initialize( );
public:
	void setScene( SCENE scene );
	void setSelectSong( int select, Songs::DIFF diff );
	void run( );
	void setFinish( );
	void setResult( RESULT result );
	void setKey( SE se );
	void setKey( KEY key );
	bool isPushKey( int key ) const;
	bool isHoldKey( int key ) const;
	bool isPushDevice( int key ) const;
	bool isHoldDevice( int key ) const;
	bool isNext( ) const;
	bool isBack( ) const;
	bool isDongLeft( ) const;
	bool isDongRight( ) const;
	bool isKaLeft( ) const;
	bool isKaRight( ) const;
	bool isHoldKaLeft( ) const;
	bool isHoldKaRight( ) const;
	bool isAutomatic( ) const;
private:
	void changeAutomatic( );
	bool isFinish( );
	void update( );
	void flip( ) const;
	GamePtr getThis( );
	void changeScene( SCENE scene );
	void updateKey( );
	void updateDevice( );
	void updateSe( );
private:
	SCENE _scene;
	SCENE _old_scene;
	ScenePtr _scene_ptr;
	SongsPtr _songs;
	std::array< int, GRAPH_MAX > _images;
	int _selecting_song;
	int _se[ SE::MAX_SE ];
	RESULT _result;
	Songs::DIFF _diff;
	bool _is_finish;
	bool _auto;
	bool _automatic[ SE::MAX_SE ];
	char _key[ 256 ];
	char _device[ 256 ];
};