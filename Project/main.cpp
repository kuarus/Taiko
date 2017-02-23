#include "Application.h"
#include "Drawer.h"
#include "Sound.h"
#include "Keyboard.h"
#include "Device.h"
#include "Game.h"
#include "define.h"

void main( ) {
	TaskPtr drawer = DrawerPtr( new Drawer( "Resources" ) );
	TaskPtr sound = SoundPtr( new Sound( "Sound" ) );
	TaskPtr keyboard = KeyboardPtr( new Keyboard ); 
	TaskPtr device = DevicePtr( new Device );
	TaskPtr game = GamePtr( new Game );

	ApplicationPtr app = Application::getInstance( );
	app->setWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );

	app->addTask( Drawer::getTag( ), drawer );
	app->addTask( Sound::getTag( ), sound );
	app->addTask( Game::getTag( ), game );
	app->addTask( Keyboard::getTag( ), keyboard );
	app->addTask( Device::getTag( ), device );
}