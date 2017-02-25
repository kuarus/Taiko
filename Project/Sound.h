#pragma once
class Sound {
public:
	Sound( );
	virtual ~Sound( );
public:
	static int load( const char* filename );
	static void playSE( int handle, bool loop );
	static void playSound( int handle, bool loop );
	static void stop( int handle );
	static void destroy( int handle );
	static int getTime( int handle );
	static void changeVol( int vol, int handle );
};

