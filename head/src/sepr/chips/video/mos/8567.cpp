#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/video/video.hpp"
#include "8567.hpp"

cVideo_Mos_8567::cVideo_Mos_8567( std::string pName, cSepr *pSepr, cDevice *pParent ) : cVideo("VIC-II", pSepr, pParent, 320, 200, 1 ) {
	
	mScaleSet(2);
}

cVideo_Mos_8567::~cVideo_Mos_8567() {
	
}

void cVideo_Mos_8567::cycle() {
	

}

void cVideo_Mos_8567::reset() {
	_vidBaseSrc = _vidBaseChar	 = _vidBaseBitmap	= 0;
	_vidSrc		= _vidChar		 = _vidBitmap		= 0;

	_rRowCounter = 7;
	_rRasterY = 0xffff;
	_rRasterInterruptY = 0;

	for(unsigned int x = 0; x < 8; x++ )
		_rMobX[x] = _rMobY[x] = _rMobColor[x] = 0;

	_rMobEnabled	=	_rMobYExpansion		= _rMobDataPriority		= _rMobMultiColor	= 0;
	_rMobXExpansion	=	_rMobCollisionMob	= _rMobCollisionData	= _rMobXBit8		= 0;
	_rMobMultiColors[0] = _rMobMultiColors[1] = 0;
			
	_rInterrupt = _rInterruptEnabled = 0;				

	_rControl1		= _rControl2 = 0;						
	_rMemoryPtrs	= 0;		
			
	_rBorderColor = 0;		
}

void cVideo_Mos_8567::paletteLoad() {
	const byte C64pal[16][3] = {
		0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 
		0x68, 0x37, 0x2b, 
		0x70, 0xa4, 0xb2, 
		0x6f, 0x3d, 0x86, 
		0x58, 0x8d, 0x43, 
		0x35, 0x28, 0x79, 
		0xb8, 0xc7, 0x6f, 
		0x6f, 0x4f, 0x25,
		0x43, 0x39, 0x00,
		0x9a, 0x67, 0x59,
		0x44, 0x44, 0x44,
		0x6c, 0x6c, 0x6c,
		0x9a, 0xd2, 0x84,
		0x6c, 0x5e, 0xb5,
		0x95, 0x95, 0x95
	};
	
	// Create the 32bit palette
	delete mPalette;
	mPaletteColors = 16;
	mPalette = new dword[ mPaletteColors ];

	// Temporary surface to convert colors
	SDL_Surface *surface = SDL_CreateRGBSurface(	SDL_SWSURFACE,	1,	1,	 32, 0, 0, 0, 0);

	// map each palette color to its sdl surface color
	for(size_t color=0; color < mPaletteColors; color++)
		mPalette[ color ] = SDL_MapRGB (	surface->format , C64pal[color][0], C64pal[color][1], C64pal[color][2] ) ;

	// cleanup
	SDL_FreeSurface( surface );
}

byte cVideo_Mos_8567::busReadByte( size_t pAddress ) {
	pAddress &= 0x3F;

	switch(pAddress) {
			
		case 0x00: case 0x02: case 0x04: case 0x06:		// MOBs 0-7 X Coord
		case 0x08: case 0x0A: case 0x0C: case 0x0E:
			return _rMobX[pAddress>>1] & 0xFF;

		case 0x01: case 0x03: case 0x05: case 0x07:		// MOBs 0-7 Y Coord
		case 0x09: case 0x0B: case 0x0D: case 0x0F:
			return _rMobY[pAddress>>1] & 0xFF;

		case 0x10:
			return _rMobXBit8;

		case 0x11:		// Control Register 1 (bit7 is rasterY)
			// Return bits 0-6 are control register, 
			// take bit 8 from rasterY and add as bit7
			return ((_rControl1 & 0x7F) | ((_rRasterY & 0x100) >> 1)) ;
			
		case 0x12:		// Raster
			return (byte) _rRasterY;

		case 0x13:		// Lightpen X
		case 0x14:		// Lightpen Y
			return 0;

		case 0x15:		// MOBs
			return _rMobEnabled;
			
		case 0x16:		// Control Register 2
			return (_rControl2 | 0xC0);

		case 0x17:		// MOB Y Expansion
			return _rMobYExpansion;

		case 0x18:		// Memory Pointers
			return (_rMemoryPtrs | 0x01);

		case 0x19:		// Interrupt Register
			return (_rInterrupt | 0x70);

		case 0x1A:		// Interrupt Enabled
			return (_rInterruptEnabled | 0xF0);

		case 0x1B:		// MOBs data Priority
			return _rMobDataPriority;

		case 0x1C:		// MOB Multi Color
			return _rMobMultiColor;

		case 0x1D:		// MOB X Expansion
			return _rMobXExpansion;

		case 0x1E:		{// MOB on MOB Collision
			byte value = _rMobCollisionMob;
			_rMobCollisionMob = 0;
			return value;
						}
		case 0x1F:		{// MOB on data Collision
			byte value = _rMobCollisionData;
			_rMobCollisionData = 0;
			return value;
						}
		case 0x20:		// Border Color (Upper4bits return 1111)
			return (_rBorderColor | 0xF0);


		case 0x21:		// Background color (Upper4bits return 1111)
		case 0x22:
		case 0x23:
		case 0x24:
			pAddress &= 0x07;
			return (_rBackgroundColor[pAddress - 1] | 0xF0);

		case 0x25:
		case 0x26:
			pAddress &= 0x03;
			return (_rMobMultiColors[pAddress-1] | 0xF0);
			
		case 0x27: case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: 
			pAddress &= 0x7;
			return (_rMobColor[pAddress-1] | 0xF0);

	}

	return 0;
}

void cVideo_Mos_8567::busWriteByte( size_t pAddress, byte pData ) {
	pAddress &= 0x3F;

	switch(pAddress) {
		case 0x10:			// MOBs X
			_rMobXBit8 = pData;
			// FIXME: need to manage the _rMobX array
			break;

		case 0x11:			// Control 1
			_rControl1 = pData;

			// Take bit7 from control register, shift it left to bit8
			// keep old bits0-7, and add it to the RasterIRQ
			_rRasterInterruptY = (_rRasterInterruptY & 0xFF) | ((pData & 0x80) << 1);
			break;

		case 0x12:			// Raster Counter

			// Keep old Bit8, add new byte
			_rRasterInterruptY = (_rRasterInterruptY & 0xFF00) | pData;
			break;

		case 0x13:			// Lightpen X
			break;
		case 0x14:			// Lightpen Y
			break;

		case 0x15:			// MOBs Enabled
			_rMobEnabled = pData;
			break;
			
		case 0x16:			// Control Register 2
			_rControl2 = (pData & 0x3F);
			break;

		case 0x17:
			_rMobYExpansion = pData;
			break;

		case 0x18:
			_rMemoryPtrs	=  pData;

			_vidBaseSrc			= (pData	& 0xf0) << 6;
			_vidBaseChar		= (pData	& 0x0e) << 10;
			_vidBaseBitmap		= (pData	& 0x08) << 10;

			if(!_vidSrc)
				_vidSrc = _vidBaseSrc;

			break;

		case 0x19:			// Interrupt Register
			// A 1 is written into the interrupt flag that we want to disable
			// Get the bits we need to keep, and AND with the current
			_rInterrupt &= (~pData & 0x0F);
			//board()->interruptClear(this);

			// Check for an active interrupt against the mask
			if( _rInterrupt & _rInterruptEnabled)
				_rInterrupt |= 0x80;				// IRQ Waiting Bit

			break;

		case 0x1A:			// Interrupt Enabled
			_rInterruptEnabled = (pData & 0x0F);
			// Check for an active interrupt against the mask
			if( _rInterrupt & _rInterruptEnabled) {
				_rInterrupt |= 0x80;				// IRQ Waiting Bit
				//board()->interruptFire( false, this );
			} else {
				_rInterrupt &= 0x7F;				// Only keep lower 4 bits (IRQ Waiting Bit off)
				//board()->interruptClear(this);
			}
			break;
			
		case 0x1B:
			_rMobDataPriority = pData;
			break;

		case 0x1C:
			_rMobMultiColor = pData;
			break;

		case 0x1D:
			_rMobXExpansion = pData;
			break;

		case 0x1E:			// MOB on MOB Col
		case 0x1F:			// MOB on DTA Col
			break;

		case 0x20:			// Border Color
			_rBorderColor = (pData & 0x0F);
			break;
							// Background Colors
		case 0x21:	case 0x22:  case 0x23:  case 0x24:
			pAddress &= 0x07;
			_rBackgroundColor[pAddress - 1] = (pData & 0x0F);		// Only lower4bits can be used
			break;

		case 0x25:
		case 0x26:
			pAddress &= 0x3;
			_rMobMultiColors[pAddress - 1] = (pData & 0x0F);
			break;

		case 0x27: case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E:
			pAddress &= 0x7;
			_rMobColor[pAddress - 1] = (pData & 0x0F);
			break;
	}

}
