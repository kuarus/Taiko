#pragma once
class Sound {
public:
	Sound( );
	virtual ~Sound( );
public:
	static int load( const char* filename );
	static void playSE( int handle, bool loop );
	static void playMusic( int handle, bool loop, int position = 1 );
	static bool isPlayingMusic( int handle );
	static void stop( int handle );
	static void destroy( int handle );
	static int getTime( int handle );
	static void changeVol( int vol, int handle );
};

