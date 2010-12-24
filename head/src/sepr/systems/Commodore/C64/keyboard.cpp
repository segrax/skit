#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "input/keyboard/keyboard.hpp"
#include "keyboard.hpp"

cCommodore_64_Keyboard::cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cDevice *pParent ) : cKeyboard( pName, pSepr, pParent ) {

}

void cCommodore_64_Keyboard::releaseKey( size_t pKey ) {
	cKeyboard::releaseKey( pKey );

}

void cCommodore_64_Keyboard::pressKey( size_t pKey ) {
	cKeyboard::pressKey( pKey );


}
