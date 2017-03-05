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
	SetAlwaysRunFlag( TRUE );
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
	_se[ SE::SE_DONG ] = Sound::load( "Resource/Sound/dong.wav" );
	_se[ SE::SE_KA   ] = Sound::load( "Resource/Sound/ka.wav" );

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

void Game::setKey( SE se ) {
	if ( se == SE::SE_DONG ) {
		_automatic[ SE::SE_DONG ] = true; 
	}
	if ( se == SE::SE_KA ) {
		_automatic[ SE::SE_KA ] = true; 
	}
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
	bool push = isPushKey( KEY::KEY_J );
	if ( _automatic[ SE::SE_DONG ] ) {
		push = true;
	}
	return push;
}

bool Game::isKaLeft( ) const {
	bool push = false;
	if ( isPushKey( KEY::KEY_D ) ) {
		push = true;
	}
	if ( isPushKey( KEY::KEY_LEFT ) ) {
		push = true;
	}
	if ( _automatic[ SE::SE_KA ] ) {
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

bool Game::isAutomatic( ) const {
	return _auto;
}


void Game::changeAutomatic( ) {
	if ( isPushKey( KEY::KEY_1 ) ) {
		_auto = !_auto;
	}
}

void Game::setScene( SCENE scene ) {
	_scene = scene;
}

void Game::setSelectSong( int select, Songs::DIFF diff ) {
	_selecting_song = select;
	_diff = diff;
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
	changeAutomatic( );
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
		_scene_ptr.reset( new ScenePlay( _selecting_song, _songs, _diff ) );
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
		Sound::playSE( _se[ SE::SE_DONG ], false );
	}
	if ( isDongRight( ) ) {
		Sound::playSE( _se[ SE::SE_DONG ], false );
	}
	if ( isKaLeft( ) ) {
		Sound::playSE( _se[ SE::SE_KA ], false );
	}
	if ( isKaRight( ) ) {
		Sound::playSE( _se[ SE::SE_KA ], false );
	}
	if ( isPushKey( KEY::KEY_SPACE ) ) {
		Sound::playSE( _se[ SE::SE_DONG ], false );
	}
	_automatic[ SE::SE_DONG ] = false;
	_automatic[ SE::SE_KA ] = false;
}