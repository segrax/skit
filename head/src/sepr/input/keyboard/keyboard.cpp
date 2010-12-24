#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "keyboard.hpp"

cKeyboard::cKeyboard( std::string pName, cSepr *pSepr, cDevice *pParent ) : cDevice( pName, pSepr, pParent ) {

}

size_t cKeyboard::cycle() {

	return 0;
}

void cKeyboard::pressKey( size_t pKey ) {

	mKeyPressed[ pKey ].mPressed = true; 
}

void cKeyboard::releaseKey( size_t pKey ) {
	mKeyPressed[ pKey ].mCycles = 10;
	mKeyPressed[ pKey ].mPressed = false;
}

bool cKeyboard::isPressed( size_t pKey ) { 

	return mKeyPressed[ pKey ].mPressed; 
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
