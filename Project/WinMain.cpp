#include "Game.h"
#include <Windows.h>
#include <memory>

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int ) {
	std::shared_ptr< Game >( new Game )->run( );
	return 0;
}