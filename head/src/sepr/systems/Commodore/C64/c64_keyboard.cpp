#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/interfaceAdapter/mos/6526.hpp"
#include "input/keyboard/keyboard.hpp"
#include "c64_keyboard.hpp"


cCommodore_64_Keyboard::cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cDevice *pParent ) : cKeyboard( pName, pSepr, pParent ), cCia_Mos_6526( pName, pSepr, pParent ) {

	for( int i = 0; i < 8; ++i ) {
		mKeysRow[i] = 0xFF;
		mKeysCol[i] = 0xFF;
	}
}

byte cCommodore_64_Keyboard::busReadByte( size_t pAddress ) {

	switch( pAddress & 0x01 ) {
		case 0x00:{
			byte ret = mRegPeripheralDataA | ~mRegDataDirectionA, tst = (mRegPeripheralDataB | ~mRegDataDirectionB);// & Joystick1;
			if (!(tst & 0x01)) ret &= mKeysCol[0];	// AND all active columns
			if (!(tst & 0x02)) ret &= mKeysCol[1];
			if (!(tst & 0x04)) ret &= mKeysCol[2];
			if (!(tst & 0x08)) ret &= mKeysCol[3];
			if (!(tst & 0x10)) ret &= mKeysCol[4];
			if (!(tst & 0x20)) ret &= mKeysCol[5];
			if (!(tst & 0x40)) ret &= mKeysCol[6];
			if (!(tst & 0x80)) ret &= mKeysCol[7];
			return ret;// & Joystick2;
				  }

		case 0x01: {
			byte ret = ~mRegDataDirectionB, tst = (mRegPeripheralDataA | ~mRegDataDirectionA);// & Joystick2;
			if (!(tst & 0x01)) ret &= mKeysRow[0];	// AND all active rows
			if (!(tst & 0x02)) ret &= mKeysRow[1];
			if (!(tst & 0x04)) ret &= mKeysRow[2];
			if (!(tst & 0x08)) ret &= mKeysRow[3];
			if (!(tst & 0x10)) ret &= mKeysRow[4];
			if (!(tst & 0x20)) ret &= mKeysRow[5];
			if (!(tst & 0x40)) ret &= mKeysRow[6];
			if (!(tst & 0x80)) ret &= mKeysRow[7];
			return (ret | (mRegPeripheralDataB & mRegDataDirectionB));// & Joystick1;
				   }
	}

	return cCia_Mos_6526::busReadByte( pAddress );
}

void cCommodore_64_Keyboard::busWriteByte( size_t pAddress, byte pData ) {

	cCia_Mos_6526::busWriteByte( pAddress, pData );
}

void cCommodore_64_Keyboard::pressKey( SDL_keysym pKey ) {
	cKeyboard::pressKey( pKey );

	size_t c64key = SDLKeyToC64( pKey );
	if(c64key == -1)
		return;

	int c64_byte = c64key >> 3;
	int c64_bit = c64key & 7;
	int shifted = c64key & 128;
	c64_byte &= 7;
	if (shifted) {
		mKeysRow[6] &= 0xef;
		mKeysCol[4] &= 0xbf;
	}
	mKeysRow[c64_byte] &= ~(1 << c64_bit);
	mKeysCol[c64_bit] &= ~(1 << c64_byte);

}

void cCommodore_64_Keyboard::releaseKey( SDL_keysym pKey ) {
	cKeyboard::releaseKey( pKey );

	size_t c64key = SDLKeyToC64( pKey );

	int c64_byte = c64key >> 3;
	int c64_bit = c64key & 7;
	int shifted = c64key & 128;
	c64_byte &= 7;
	if (shifted) {
		mKeysRow[6] |= 0x10;
		mKeysCol[4] |= 0x40;
	}
	mKeysRow[c64_byte] |= (1 << c64_bit);
	mKeysCol[c64_bit] |= (1 << c64_byte);
}

#define MATRIX(a,b) (((a) << 3) | (b))
#include<iostream>

size_t cCommodore_64_Keyboard::SDLKeyToC64( SDL_keysym pKey ) {
	int result = -1;
	
	if( pKey.mod == KMOD_LSHIFT || pKey.mod == KMOD_RSHIFT ) {
		switch( toupper(pKey.scancode ) ) {
		case 3:
			return MATRIX(7,3) | 0x80;
		}

		return -1;
	}

	switch ( toupper(pKey.sym) ) {
	case VK_RETURN: return MATRIX(0,1);
	case VK_F1: return MATRIX(0,4);
	case VK_F2: return MATRIX(0,4) | 0x80;
	case VK_F3: return MATRIX(0,5);
	case VK_F4: return MATRIX(0,5) | 0x80;
	case VK_F5: return MATRIX(0,6);
	case VK_F6: return MATRIX(0,6) | 0x80;
	case VK_F7: return MATRIX(0,3);
	case VK_F8: return MATRIX(0,3) | 0x80;

	case '0': return MATRIX(4,3);
	case '1': return MATRIX(7,0);
	case '2': return MATRIX(7,3);
	case '3': return MATRIX(1,0);
	case '4': return MATRIX(1,3);
	case '5': return MATRIX(2,0);
	case '6': return MATRIX(2,3);
	case '7': return MATRIX(3,0);
	case '8': return MATRIX(3,3);
	case '9': return MATRIX(4,0);
	
	case SDLK_COMMA: return MATRIX(5,7);
	/*case VK_bracketleft: return MATRIX(5,6);
	case VK_bracketright: return MATRIX(6,1);
	case VK_slash: return MATRIX(6,7);
	case VK_semicolon: return MATRIX(5,5);
	case VK_grave: return MATRIX(7,1);
	case VK_minus: return MATRIX(5,0);
	case VK_equal: return MATRIX(5,3);
	
	case VK_period: return MATRIX(5,4);
	case VK_backslash: return MATRIX(6,6);
	*/
	

	case 'A': result = MATRIX(1,2); break;
	case 'B': result = MATRIX(3,4); break;
	case 'C': result = MATRIX(2,4); break;
	case 'D': result = MATRIX(2,2); break;
	case 'E': result = MATRIX(1,6); break;
	case 'F': result = MATRIX(2,5); break;
	case 'G': result = MATRIX(3,2); break;
	case 'H': result = MATRIX(3,5); break;
	case 'I': result = MATRIX(4,1); break;
	case 'J': result = MATRIX(4,2); break;
	case 'K': result = MATRIX(4,5); break;
	case 'L': result = MATRIX(5,2); break;
	case 'M': result = MATRIX(4,4); break;
	case 'N': result = MATRIX(4,7); break;
	case 'O': result = MATRIX(4,6); break;
	case 'P': result = MATRIX(5,1); break;
	case 'Q': result = MATRIX(7,6); break;
	case 'R': result = MATRIX(2,1); break;
	case 'S': result = MATRIX(1,5); break;
	case 'T': result = MATRIX(2,6); break;
	case 'U': result = MATRIX(3,6); break;
	case 'V': result = MATRIX(3,7); break;
	case 'W': result = MATRIX(1,1); break;
	case 'X': result = MATRIX(2,7); break;
	case 'Y': result = MATRIX(3,1); break;
	case 'Z': result = MATRIX(1,4); break;

	}

	if (result != -1 && GetKeyState(VK_CAPITAL))
		result |= 0x80;

	return result;
}