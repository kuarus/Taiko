#pragma once
#include "Scene.h"
#include <list>
#include <string>
#include "Bullet.h"

PTR( Songs );
PTR( Bullet );

class ScenePlay : public Scene {
public:
	ScenePlay( int select, SongsPtr songs, Songs::DIFF diff );
	virtual ~ScenePlay( );
public:
	void update( GamePtr game );
	void draw( GamePtr game );
private:
	enum MOOD {
		MOOD_NORMAL,
		MOOD_CLEAR,
		MOOD_HOT,
		MOOD_MISS,
		MOOD_MISS_CLEAR,
		MAX_STATE
	};
	enum PLAY_STATE {
		PLAY_STATE_WAIT,
		PLAY_STATE_START,
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
		IMAGE_COMBO_NUM,
		IMAGE_SCORE_NUM,
		IMAGE_JUDGE,
		IMAGE_EXPLOSION,
		IMAGE_FLASH_RED,
		IMAGE_FLASH_BLUE,
		IMAGE_NORMAGAUGE,
		IMAGE_SOUL,
		MAX_IMAGE
	};
	enum COMBO {
		COMBO_50,
		COMBO_100,
		COMBO_200,
		COMBO_300,
		COMBO_400,
		COMBO_500,
		COMBO_600,
		COMBO_700,
		COMBO_800,
		COMBO_900,
		COMBO_FULL,
		MAX_COMBO,
	};
private:
	void loadImages( );
	void loadSounds( );
	void updateStart( GamePtr game );
	void updatePlay( GamePtr game );
	void updateJudge( );
	void updateBullet( int idx, GamePtr game );
	void drawBg( );
	void drawBarLine( ) const;
	void drawBullet( ) const;
	void drawTitle( ) const;
	void drawMTaikoBack( ) const;
	void drawMTaikoFront( GamePtr game ) const;
	void drawJudge( ) const;
	void drawExplosion( );
	void drawFlash( GamePtr game );
	void drawCombo( int combo ) const;
	void drawScore( int score ) const;
	void drawSoulGauge( ) const;
	void drawNote( GamePtr game ) const;
	void loadBullet( SongsPtr songs, int select, Songs::DIFF diff );
	void setJudge( Bullet::JUDGE judge );
	void creatBullet( );
	void playComboSound( );
	void addScore( );
private:
	MOOD _mood;
	PLAY_STATE _play_state;
	Game::RESULT _result;
	Songs::SONG_DATA _song;
	Bullet::JUDGE _judge;
	Bullet::JUDGE _judge_draw;
	Bullet::TYPE _flash_type;
	Bullet::CODE _start_code;
	bool _start;
	int _idx;
	int _music;
	int _before_seq;
	int _count;
	int _flash_count;
	int _image[ IMAGE::MAX_IMAGE ];
	int _combo_sound[ COMBO::MAX_COMBO ];
	std::string _title;
	std::list< BulletPtr > _bullets;
	std::vector< Bullet::CODE > _codes;
};

