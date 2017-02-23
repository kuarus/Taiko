#pragma once
class Sound {
public:
	Sound( );
	virtual ~Sound( );
public:
	static int loadSound( const char* filename );
	static void playSound( int handle, bool loop );
	static void stopSound( int handle );
	static void deleteSound( int handle );
	static int getSoundPosition( int handle );
	static void changeVol( int vol, int handle );
};

