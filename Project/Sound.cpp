#include "Sound.h"
#include "DxLib.h"

Sound::Sound( ) {
}


Sound::~Sound( ) {
}


int Sound::load( const char* filename ) {
	return LoadSoundMem( filename );
}

void Sound::playSE( int handle, bool loop ) {
	int dx_type = DX_PLAYTYPE_BACK;
	if ( loop ) {
		dx_type = DX_PLAYTYPE_LOOP;
	}
	PlaySoundMem( handle, dx_type );
}

void Sound::playMusic( int handle, bool loop, int position ) {
	int dx_type = DX_PLAYTYPE_BACK;
	if ( loop ) {
		dx_type = DX_PLAYTYPE_LOOP;
	}
	PlayStreamSoundMem( handle, dx_type, position );
}

bool Sound::isPlayingMusic( int handle ) {
	bool result = false;
	if ( CheckSoundMem( handle ) ) {
		result = true;
	}
	return result;
}

void Sound::stop( int handle ) {
	StopSoundMem( handle );
}

void Sound::destroy( int handle ) {
	DeleteSoundMem( handle );
}

int Sound::getTime( int handle ) {
	return GetSoundCurrentTime( handle );
}

void Sound::changeVol( int vol, int handle ) {
	ChangeVolumeSoundMem( vol, handle );
}