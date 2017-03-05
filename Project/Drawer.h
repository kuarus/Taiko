#pragma once
#include <string>

class Drawer {
public:
	Drawer( );
	virtual ~Drawer( );
public:
	static void drawGraph( int tx, int ty, int sx1, int sy1, int sx2, int sy2, int width, int height, int image, int param = 255 );
	static void drawBox( int x1, int y1, int x2, int y2, unsigned int color );
	static unsigned int getColor( int red, int green, int blue );
	static unsigned int getColor( std::string color_code );
	static void drawVString( int x, int y, const char* str, bool selecting = false );
	static void drawString( int x, int y, const char* str, ... );
	static int loadGraph( const char* filename );
	static void deleteGraph( int handle );
	static void changeFont( const char* type );
	static void drawLine( int x1, int y1, int x2, int y2 );
};