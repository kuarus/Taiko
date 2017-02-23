#include "Sound.h"
#include "DxLib.h"

Sound::Sound( ) {
}


Sound::~Sound( ) {
}


int Sound::loadSound( const char* filename ) {
	return LoadSoundMem( filename );
}

void Sound::playSound( int handle, bool loop ) {
	int dx_type = DX_PLAYTYPE_BACK;
	if ( loop ) {
		dx_type = DX_PLAYTYPE_LOOP;
	}
	PlaySoundMem( handle, dx_type );
}

void Sound::stopSound( int handle ) {
	StopSoundMem( handle );
}

void Sound::deleteSound( int handle ) {
	DeleteSoundMem( handle );
}

int Sound::getSoundPosition( int handle ) {
	return GetSoundCurrentPosition( handle );
}

void Sound::changeVol( int vol, int handle ) {
	ChangeVolumeSoundMem( vol, handle );
}