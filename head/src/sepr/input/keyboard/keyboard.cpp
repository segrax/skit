#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "keyboard.hpp"

cKeyboard::cKeyboard( std::string pName, cSepr *pSepr, cDevice *pParent ) : cDevice( pName, pSepr, pParent ) {
	
	for( int i = 0; i < 0x100; ++i )
		mKeyPressed[i] = false;

}

size_t cKeyboard::cycle() {

	return 0;
}

void cKeyboard::pressKey( size_t pKey ) {

	mKeyPressed[ pKey ] = true; 
}

void cKeyboard::releaseKey( size_t pKey ) {

	mKeyPressed[ pKey ] = false;
}

bool cKeyboard::isPressed( size_t pKey ) { 

	return mKeyPressed[ pKey ]; 
}

void cKeyboard::sdlEvent( SDL_KeyboardEvent *pEvent ) {

	switch( pEvent->state ) {
	
		case SDL_PRESSED:
			pressKey( pEvent->keysym.sym);
			break;

		case SDL_RELEASED:
			releaseKey( pEvent->keysym.sym);
			break;
	}
	
}
