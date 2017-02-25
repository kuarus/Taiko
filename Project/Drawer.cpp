#include "Drawer.h"
#include "DxLib.h"

Drawer::Drawer( ) {
}


Drawer::~Drawer( ) {
}

void Drawer::drawGraph( int tx, int ty, int sx1, int sy1, int sx2, int sy2, int width, int height, int image ) {
	int cx = tx * width;
	int cy = ty * height;
	DrawRectExtendGraph( sx1, sy1, sx2, sy2, cx, cy, width, height, image, TRUE );
}

void Drawer::drawBox( int x1, int y1, int x2, int y2, unsigned int color ) {
	int diff = 4;
	DrawBox( x1, y1, x2, y2, color, TRUE );
	DrawBox( x1 + diff, y1 + diff, x2 - diff, y2 - diff, color - GetColor( 50, 50, 50 ), TRUE );
}

unsigned int Drawer::getColor( int red, int green, int blue ) {
	return GetColor( red, green, blue );
}

void Drawer::drawVString( int x, int y, const char* str, bool selecting ) {
	int color = GetColor( 255, 255, 255 );
	if ( selecting ) {
		color = GetColor( 100, 0, 100 );
	}
	DrawVString( x, y, str, color, GetColor( 0, 0, 0 ) );
}

void Drawer::drawString( int x, int y, const char* str, ... ) {
	char buf[ 1024 ];
	va_list list;
	va_start( list, str );
	vsprintf_s( buf, 1024, str, list );
	DrawString( x, y, buf, GetColor( 255, 255, 255 ), GetColor( 0, 0, 0 ) );
	va_end( list );
}

int Drawer::loadGraph( const char* filename ) {
	return LoadGraph( filename );
}

void Drawer::deleteGraph( int handle ) {
	DeleteGraph( handle );
}

void Drawer::changeFont( const char* type ) {
	ChangeFont( type );
}

void Drawer::drawLine( int x1, int y1, int x2, int y2 ) {
	DrawLine( x1,y1, x2, y2, GetColor( 255, 255, 255 ), 1 );
}