#include "Game.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneMenu.h"
#include "SceneOption.h"
#include "SceneSongSelect.h"
#include "ScenePlay.h"
#include "define.h"
#include <assert.h>
#include "Drawer.h"

Game::Game( ) :
_scene( SCENE::SCENE_TITLE ),
_old_scene( _scene ),
_selecting_song( 0 ),
_is_finish( false ){
	if ( DxLib_Init( ) == -1 ) {
		return;
	}

	SetGraphMode( WINDOW_WIDTH, WINDOW_HEIGHT, 16 );
	//SetBackgroundColor( 200, 0, 60 );
	SetDrawScreen( DX_SCREEN_BACK );
	SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	ChangeWindowMode( TRUE );
	SetFontSize( FONT_SIZE );

	for ( int i = 0; i < GRAPH_MAX; i++ ) {
		_images[ i ] = 0;
	}
	_sound_dong = LoadSoundMem( "Resource/Sound/dong.wav" );
	_sound_ka = LoadSoundMem( "Resource/Sound/ka.wav" );

	_songs = SongsPtr( new Songs );
	_scene_ptr = ScenePtr( new SceneTitle( ) );
	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE );
}

Game::~Game( ) {
	DxLib_End( );
}

void Game::run( ) {
	while ( !ProcessMessage( ) && !isFinish( ) ) {
		ClearDrawScreen( );
		update( );
		ScreenFlip( );
	}
}

void Game::setFinish( ) {
	_is_finish = true;
}

bool Game::isPushKey( int key ) {
	return _key[ key ] == 1;
}

bool Game::isHoldKey( int key ) {
	return _key[ key ] > 1;
}

bool Game::isNext( ) {
	bool push = false;
	if ( isPushKey( KEY::KEY_F ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_J ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_SPACE ) ) {
		push = true;
	}
	return push;
}

bool Game::isBack( ) {
	bool push = false;
	if ( isPushKey( KEY::KEY_Q ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_BACK ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_ESCAPE ) ) {
		push = true;
	}

	if ( push ) {
		PlaySoundMem( _sound_ka, DX_PLAYTYPE_BACK );
	}
	return push;
}

bool Game::isLeft( ) {
	bool push = false;
	if ( isPushKey( KEY::KEY_D ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_LEFT ) ) {
		push = true;
	}
	return push;
}

bool Game::isRight( ) {
	bool push = false;
	if ( isPushKey( KEY::KEY_K ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_RIGHT ) ) {
		push = true;
	}
	return push;
}

void Game::setScene( SCENE scene ) {
	_scene = scene;
}

void Game::setSelectSong( int select ) {
	_selecting_song = select;
}

bool Game::isFinish( ) {
	if ( _scene == SCENE_TITLE ) {
		if ( isBack( ) ) {
			_is_finish = true;
		}
	}
	return _is_finish;
}

void Game::update( ) {
	if ( _scene_ptr ) {
		_scene_ptr->update( getThis( ) );
		_scene_ptr->draw( );
	}
	if ( _old_scene != _scene ) {
		changeScene( _scene );
		_old_scene = _scene;
	}
	updateKey( );
	updateSE( );
}

void Game::flip( ) {
}

GamePtr Game::getThis( ) {
	return shared_from_this( );
}

void Game::changeScene( SCENE scene ) {
	switch ( scene ) {
	case SCENE::SCENE_TITLE:
		_scene_ptr.reset( new SceneTitle( ) );
		break;
	case SCENE::SCENE_MENU:
		_scene_ptr.reset( new SceneMenu( ) );
		break;
	case SCENE::SCENE_OPTION:
		_scene_ptr.reset( new SceneOption( ) );
		break;
	case SCENE::SCENE_SONG_SELECT:
		_scene_ptr.reset( new SceneSongSelect( _songs, _selecting_song ) );
		break;
	case SCENE::SCENE_PLAY:
		_scene_ptr.reset( new ScenePlay( _selecting_song, _songs ) );
	}
}

void Game::updateSE( ) {
	if ( isPushKey( KEY::KEY_F ) ) {
		PlaySoundMem( _sound_dong, DX_PLAYTYPE_BACK );
	}
	if ( isPushKey( KEY::KEY_J ) ) {
		PlaySoundMem( _sound_dong, DX_PLAYTYPE_BACK );
	}
	if ( isLeft( ) ) {
		PlaySoundMem( _sound_ka, DX_PLAYTYPE_BACK );
	}
	if ( isRight( ) ) {
		PlaySoundMem( _sound_ka, DX_PLAYTYPE_BACK );
	}

	if ( isPushKey( KEY::KEY_SPACE ) ) {
		PlaySoundMem( _sound_dong, DX_PLAYTYPE_BACK );
	}
}

void Game::updateKey( ) {
	char key[ 256 ];
	GetHitKeyStateAll( key );
	for ( int i = 0; i < 256; i++ ) {
		if ( key[ i ] == 0 ) {
			_key[ i ] = 0;
		}
		_key[ i ] += key[ i ];
	}
}

