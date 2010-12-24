#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "keyboard.hpp"

cKeyboard::cKeyboard( std::string pName, cSepr *pSepr, cDevice *pParent ) : cDevice( pName, pSepr, pParent ) {
	
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
			mKeyPressed[ pEvent->keysym.scancode ] = true;
			break;

		case SDL_RELEASED:
			mKeyPressed[ pEvent->keysym.scancode ] = false;
			break;
	}
	
}
