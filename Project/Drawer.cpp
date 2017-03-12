#include "Drawer.h"
#include "DxLib.h"
#include "define.h"
#include <algorithm>

Drawer::Drawer( ) {
}

Drawer::~Drawer( ) {
}

void Drawer::drawGraph( int tx, int ty, int sx1, int sy1, int sx2, int sy2, int width, int height, int image, int param ) {
	int cx = tx * width;
	int cy = ty * height;
	if ( param != 255 ) SetDrawBlendMode( DX_BLENDMODE_ALPHA, param );
	DrawRectExtendGraph( sx1, sy1, sx2, sy2, cx, cy, width, height, image, TRUE );
	if ( param != 255 ) SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 );
}

void Drawer::drawBox( int x1, int y1, int x2, int y2, unsigned int color ) {
	int interval = 5;
	SetDrawBright( 180, 180, 180 );
	DrawBox( x1, y1, x2, y2, color, TRUE );
	SetDrawBright( 255, 255, 255 );
	DrawBox( x1 + interval, y1 + interval, x2 - interval, y2 - interval, color, TRUE );
}

unsigned int Drawer::getColor( int red, int green, int blue ) {
	return GetColor( red, green, blue );
}

unsigned int Drawer::getColor( std::string color_code ) {
	if ( color_code.size( ) == 0 ) {
		return GetColor( 0, 100, 0 );
	}
	std::string red_str   = color_code.substr( 1, 2 );
	std::string green_str = color_code.substr( 3, 2 );
	std::string blue_str  = color_code.substr( 5, 2 );
	int red   = std::stoi( red_str  , nullptr, 16 );
	int green = std::stoi( green_str, nullptr, 16 );
	int blue  = std::stoi( blue_str , nullptr, 16 );
	return GetColor( red, green, blue );
}


void Drawer::drawVString( int x, int y, unsigned int color, const char* str, ... ) {
	int tmp_y = y;
	if ( x > WINDOW_WIDTH ||
		 y > WINDOW_HEIGHT) {
		return;
	}
	if ( x < -FONT_SIZE ||
		 y < -FONT_SIZE ) {
		return;
	}
	char buf[ 1024 ] = { 0 };
	va_list list;
	va_start( list, str );
	vsprintf_s( buf, 1024, str, list );
	std::string string( buf, 1024 );
	int size = string.size( );
	//std::string check = "";
	while ( string.at( 0 ) != 0 ) {
		std::string tmp;
		if ( IsDBCSLeadByte( string[ 0 ] ) == 0 ) {
			tmp = string.substr( 0, 1 );
			string.erase( 0, 1 );
			if ( tmp_y + FONT_SIZE * 15 < y ) {
				DrawRotaString( x, y, 1, 1, 0, FONT_SIZE, 3.14 / 2, color, GetColor( 0, 0, 0 ), false, tmp.c_str( ) );
				y -= FONT_SIZE * 6;
				x -= FONT_SIZE;
				continue;
			}
			if ( tmp < "A" ||
				 tmp > "z" )  {
				DrawRotaString( x, y, 1, 1, 0, FONT_SIZE, 3.14 / 2, color, GetColor( 0, 0, 0 ), false, tmp.c_str( ) );
				//DrawVString( x, y, tmp.c_str( ), color );
			} else {
				DrawString( x + 4, y, tmp.c_str( ), color );
			}
			y -= 2;
		} else {
			tmp = string.substr( 0, 2 );
			string.erase( 0, 2 );
			if ( tmp_y + FONT_SIZE * 15 < y ) {
				DrawRotaString( x, y, 1, 1, 0, FONT_SIZE, 3.14 / 2, color, GetColor( 0, 0, 0 ), false, tmp.c_str( ) );
				y -= FONT_SIZE * 6;
				x -= FONT_SIZE;
				continue;
			}
			if ( tmp >= "ÅN" &&
				 tmp <= "Å¸" &&
				 tmp != "Åô" &&
				 tmp != "Åö" )  {
				DrawRotaString( x, y, 1, 1, 0, FONT_SIZE, 3.14 / 2, color, GetColor( 0, 0, 0 ), false, tmp.c_str( ) );
			} else {
				DrawString( x, y, tmp.c_str( ), color );
			}
		}
		y += FONT_SIZE;
		if ( string.size( ) == 0 ) {
			break;
		}
	}
	va_end( list );
}

void Drawer::drawString( int x, int y, unsigned int color, const char* str, ... ) {
	char buf[ 1024 ];
	va_list list;
	va_start( list, str );
	vsprintf_s( buf, 1024, str, list );
	DrawString( x, y, buf, GetColor( 255, 255, 255 ), GetColor( 0, 0, 0 ) );
	va_end( list );
}

void Drawer::changeFontSize( int font_size ) {
	SetFontSize( font_size );
}

void Drawer::drawLoading( int pattern ) {
	std::string str = "ì«Ç›çûÇ›íÜ";
	for ( int i = 0; i < pattern; i++ ) {
		str += ".";
	}
	Drawer::drawBox( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Drawer::getColor( 200, 0, 20 ) );
	Drawer::drawString( 100, 200, GetColor( 255, 255, 255 ),str.c_str( ) );
	ScreenFlip( );
	ClearDrawScreen( );
}

int Drawer::loadGraph( const char* filename ) {
	return LoadGraph( filename );
}

void Drawer::deleteGraph( int handle ) {
	DeleteGraph( handle );
}

void Drawer::drawLine( int x1, int y1, int x2, int y2 ) {
	DrawLine( x1,y1, x2, y2, GetColor( 255, 255, 255 ), 1 );
}