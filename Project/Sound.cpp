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

void Sound::playSound( int handle, bool loop ) {
	int dx_type = DX_PLAYTYPE_BACK;
	if ( loop ) {
		dx_type = DX_PLAYTYPE_LOOP;
	}
	PlaySoundMem( handle, dx_type );
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