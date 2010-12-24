#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "input/keyboard/keyboard.hpp"
#include "c64_keyboard.hpp"
#include "chips/interfaceAdapter/mos/6526.hpp"

cCommodore_64_Keyboard::cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cDevice *pParent ) : cKeyboard( pName, pSepr, pParent ) {

}

size_t cCommodore_64_Keyboard::cycle() {
	cCia_Mos_6526 *dev = (cCia_Mos_6526*) mSepr->mSystemGet()->deviceGetByName("CIA1", false);

	byte col = (~dev->portAWrite()) & 0x3F;
	byte data = 0xFF;

	if(col == 0x3F)
		return 1;

	for( int i = 0; i < 0x100; ++i ) {

		if( mKeyPressed[ i ] ) {
			byte bits = keyRow(i, col);
			if(bits)
				data &= ~(1 << bits);
		}

	}

	dev->setByte( 0x01, data );

	return 1;
}

size_t cCommodore_64_Keyboard::keyRow( size_t pKey, size_t pCol ) {

	if(!pCol)
		return 1;

	if( pKey == SDLK_a ) {
		if( pCol == 0x2 )
			return 0x02;
	}

	return 0;
}
