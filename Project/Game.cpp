#include "Game.h"
#include "DxLib.h"
#include "SceneTitle.h"
#include "SceneMenu.h"
#include "SceneOption.h"
#include "SceneSongSelect.h"
#include "ScenePlay.h"
#include "SceneResult.h"
#include "define.h"
#include <assert.h>
#include "Drawer.h"
#include "Sound.h"

Game::Game( ) :
_scene( SCENE::SCENE_TITLE ),
_old_scene( _scene ),
_selecting_song( 0 ),
_is_finish( false ){

	ChangeWindowMode( TRUE );
	SetGraphMode( WINDOW_WIDTH, WINDOW_HEIGHT, 16 );
	if ( DxLib_Init( ) == -1 ) {
		return;
	}

	SetBackgroundColor( 255, 106, 106 );
	SetDrawScreen( DX_SCREEN_BACK );
	SetDrawMode( DX_DRAWMODE_BILINEAR ) ;
	SetFontSize( FONT_SIZE );

	for ( int i = 0; i < GRAPH_MAX; i++ ) {
		_images[ i ] = 0;
	}
	_sound_dong = Sound::load( "Resource/Sound/dong.wav" );
	_sound_ka = Sound::load( "Resource/Sound/ka.wav" );

	_songs = SongsPtr( new Songs );
	_scene_ptr = ScenePtr( new SceneTitle( ) );
	SetCreateSoundDataType( DX_SOUNDDATATYPE_FILE );
}

Game::~Game( ) {
	DxLib_End( );
}

void Game::run( ) {
	while ( !ProcessMessage( ) && !isFinish( ) ) {
		update( );
		flip( );
	}
}

void Game::setFinish( ) {
	_is_finish = true;
}

void Game::setResult( RESULT result ) {
	_result = result;
}

bool Game::isPushKey( int key ) const {
	return _key[ key ] == 1;
}

bool Game::isHoldKey( int key ) const {
	return _key[ key ] > 1;
}

bool Game::isNext( ) const {
	bool push = false;
	if ( isDongLeft( ) ) {
		push = true;
	}
	if ( isDongRight( ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_SPACE ) ) {
		push = true;
	}
	return push;
}

bool Game::isBack( ) const {
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
	return push;
}

bool Game::isDongLeft( ) const {
	bool result = isPushKey( KEY::KEY_F );
	return result;
}

bool Game::isDongRight( ) const {
	bool result = isPushKey( KEY::KEY_J );
	return result;
}

bool Game::isKaLeft( ) const {
	bool push = false;
	if ( isPushKey( KEY::KEY_D ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_LEFT ) ) {
		push = true;
	}
	return push;
}

bool Game::isKaRight( ) const {
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
	if ( isPushKey( KEY::KEY_ESCAPE ) ) {
		//_is_finish = true;
	}
	return _is_finish;
}

void Game::update( ) {
	updateKey( );
	updateSe( );
	if ( _scene_ptr ) {
		_scene_ptr->update( getThis( ) );
		_scene_ptr->draw( getThis( ) );
	}
	if ( _old_scene != _scene ) {
		changeScene( _scene );
		_old_scene = _scene;
	}
}

void Game::flip( ) const {
	ScreenFlip( );
	ClearDrawScreen( );
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
		break;
	case SCENE::SCENE_RESULT:
		_scene_ptr.reset( new SceneResult( _result ) );
		break;
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

void Game::updateSe( ) {
	if ( isDongLeft( ) ) {
		Sound::playSE( _sound_dong, false );
	}
	if ( isDongRight( ) ) {
		Sound::playSE( _sound_dong, false );
	}
	if ( isKaLeft( ) ) {
		Sound::playSE( _sound_ka, false );
	}
	if ( isKaRight( ) ) {
		Sound::playSE( _sound_ka, false );
	}
	if ( isPushKey( KEY::KEY_SPACE ) ) {
		Sound::playSE( _sound_dong, false );
	}
}