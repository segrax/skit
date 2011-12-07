#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/video/video.hpp"
#include "8567.hpp"
#include "systems/system.hpp"
#include "systems/Commodore/C64/c64.hpp"
#include "chips/register.hpp"
#include "chips/opcode.hpp"
#include "chips/cpu/cpu.hpp"

cVideo_Mos_8567::cVideo_Mos_8567( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cVideo(pName, pSepr, pSystem, pParent, 504, 313, 1 ) {
	mScaleSet(2);
	paletteLoad();
}

cVideo_Mos_8567::~cVideo_Mos_8567() {
	
}
void cVideo_Mos_8567::interruptRasterFire() {
	// Set rasterbit in IRQFlag as waiting
	mRegInterrupt |= 0x01;
		
	// Raster IRQ Enabled?
	if(!(mRegInterruptEnabled & 0x01))
		return;
		
	// IRQ Waiting bit
	mRegInterrupt |= 0x80;

	// Fire an interrupt
	cCpu *cpu = mSystem->deviceFind<cCpu>("CPU", true);

	cpu->interruptAdd( new cInterrupt("VIC", this) );

}

size_t cVideo_Mos_8567::cycle() {
	mCycle = 1;

	// End of screen row?
	if(mRegRowCounter == 64) {
		mRegRowCounter = 0;

		++mRegRasterY;

		// Reset video source to start of current rasterline
		if( mRegRasterY > 50)
			mVidSrc = mVidBaseSrc + (40 * (mDrawY / 8));

		// Calculate the screen destination row
		mBufferPtr = mBuffer + ((mRegRasterY * mPixelBytes) * mWidth);

		// Calculate X position on this row
		mBufferPtr += ((mRegRowCounter * 8) * mPixelBytes);
		//board()->clockPulse()
	}

	// End of screen
	if( mRegRasterY >= 312 ) {
		mRegRowCounter = 0;
		mRegRasterY = 0;
		mDrawY = 0;

		// Back to start of screen dest buffer
		mBufferPtr = mBuffer;

		// Back to start of screen src buffer
		mVidSrc = mVidBaseSrc;
		//board()->clockPulse();

	}

	// Time to fire an interrupt?
	if( mRegRasterY == mRegRasterInterruptY && mRegRowCounter == 0) {

		interruptRasterFire();
	}

	// Left & Right Border
	if( mRegRowCounter < 4 || mRegRowCounter > 43 ) {
		
		// 8 Pixels per row
		for(int i = 0; i < 8; ++i)
			*mBufferPtr++ = mRegBorderColor;

		++mRegRowCounter;
		
		// Reached end of raster?
		if( mRegRowCounter == 64 && mRegRasterY > 49)
			++mDrawY;

		return mCycle;
	}

	// Top and Bottom Border
	if(  mRegRasterY < 50 || mRegRasterY > 249 ) {

		// 8 Pixels per row
		for(int i = 0; i < 8; ++i)
			*mBufferPtr++ = mRegBorderColor;

		++mRegRowCounter;
		return mCycle;
	}
	
	if(mVidBaseSrc) {

		// ECM (Extended Color Mode)	
		if( mRegControl1 & 0x40 ) {

		} else {
			// BMM (Bitmap Mode)
			if( mRegControl1 & 0x20 ) {

				// MCM (Multi Color Mode)
				if( mRegControl2 & 0x10 ) {

				} else {

				}

			} else {
			// Text mode

				// MCM (Multi Color Mode)
				if( mRegControl2 & 0x10 ) {

				} else {
					decode_StandardText();
				}
			}
		}
	}

	++mRegRowCounter;
	return mCycle;
}

void cVideo_Mos_8567::reset() {
	mDrawY = -1;
	mVidBaseSrc = mVidBaseChar	 = mVidBaseBitmap	= 0;
	mVidSrc		= mVidChar		 = mVidBitmap		= 0;

	mRegRowCounter = 0;
	mRegRasterY = 0xffff;
	mRegRasterInterruptY = 0;

	for(unsigned int x = 0; x < 8; x++ )
		mRegMobX[x] = mRegMobY[x] = mRegMobColor[x] = 0;

	mRegMobEnabled	=	mRegMobYExpansion		= mRegMobDataPriority		= mRegMobMultiColor	= 0;
	mRegMobXExpansion	=	mRegMobCollisionMob	= mRegMobCollisionData	= mRegMobXBit8		= 0;
	mRegMobMultiColors[0] = mRegMobMultiColors[1] = 0;
			
	mRegInterrupt = mRegInterruptEnabled = 0;				

	mRegControl1		= mRegControl2 = 0;						
	mRegMemoryPtrs	= 0;		
			
	mRegBorderColor = 0;		
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
			return mRegMobX[pAddress>>1] & 0xFF;

		case 0x01: case 0x03: case 0x05: case 0x07:		// MOBs 0-7 Y Coord
		case 0x09: case 0x0B: case 0x0D: case 0x0F:
			return mRegMobY[pAddress>>1] & 0xFF;

		case 0x10:
			return mRegMobXBit8;

		case 0x11:		// Control Register 1 (bit7 is rasterY)
			// Return bits 0-6 are control register, 
			// take bit 8 from rasterY and add as bit7
			return ((mRegControl1 & 0x7F) | ((mRegRasterY & 0x100) >> 1)) ;
			
		case 0x12:		// Raster
			return (byte) mRegRasterY;

		case 0x13:		// Lightpen X
		case 0x14:		// Lightpen Y
			return 0;

		case 0x15:		// MOBs
			return mRegMobEnabled;
			
		case 0x16:		// Control Register 2
			return (mRegControl2 | 0xC0);

		case 0x17:		// MOB Y Expansion
			return mRegMobYExpansion;

		case 0x18:		// Memory Pointers
			return (mRegMemoryPtrs | 0x01);

		case 0x19:		// Interrupt Register
			return (mRegInterrupt | 0x70);

		case 0x1A:		// Interrupt Enabled
			return (mRegInterruptEnabled | 0xF0);

		case 0x1B:		// MOBs data Priority
			return mRegMobDataPriority;

		case 0x1C:		// MOB Multi Color
			return mRegMobMultiColor;

		case 0x1D:		// MOB X Expansion
			return mRegMobXExpansion;

		case 0x1E:		{// MOB on MOB Collision
			byte value = mRegMobCollisionMob;
			mRegMobCollisionMob = 0;
			return value;
						}
		case 0x1F:		{// MOB on data Collision
			byte value = mRegMobCollisionData;
			mRegMobCollisionData = 0;
			return value;
						}
		case 0x20:		// Border Color (Upper4bits return 1111)
			return (mRegBorderColor | 0xF0);


		case 0x21:		// Background color (Upper4bits return 1111)
		case 0x22:
		case 0x23:
		case 0x24:
			pAddress &= 0x07;
			return (mRegBackgroundColor[pAddress - 1] | 0xF0);

		case 0x25:
		case 0x26:
			pAddress &= 0x03;
			return (mRegMobMultiColors[pAddress-1] | 0xF0);
			
		case 0x27: case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: 
			pAddress &= 0x7;
			return (mRegMobColor[pAddress-1] | 0xF0);

	}

	return 0;
}

void cVideo_Mos_8567::busWriteByte( size_t pAddress, byte pData ) {
	pAddress &= 0x3F;

	switch(pAddress) {
		case 0x10:			// MOBs X
			mRegMobXBit8 = pData;
			// FIXME: need to manage the mRegMobX array
			break;

		case 0x11:			// Control 1
			mRegControl1 = pData;

			// Take bit7 from control register, shift it left to bit8
			// keep old bits0-7, and add it to the RasterIRQ
			mRegRasterInterruptY = (mRegRasterInterruptY & 0xFF) | ((pData & 0x80) << 1);
			break;

		case 0x12:			// Raster Counter

			// Keep old Bit8, add new byte
			mRegRasterInterruptY = (mRegRasterInterruptY & 0xFF00) | pData;
			break;

		case 0x13:			// Lightpen X
			break;
		case 0x14:			// Lightpen Y
			break;

		case 0x15:			// MOBs Enabled
			mRegMobEnabled = pData;
			break;
			
		case 0x16:			// Control Register 2
			mRegControl2 = (pData & 0x3F);
			break;

		case 0x17:
			mRegMobYExpansion = pData;
			break;

		case 0x18:
			mRegMemoryPtrs	=  pData;

			mVidBaseSrc			= (pData	& 0xf0) << 6;
			mVidBaseChar		= (pData	& 0x0e) << 10;
			mVidBaseBitmap		= (pData	& 0x08) << 10;

			if(!mVidSrc)
				mVidSrc = mVidBaseSrc;

			break;

		case 0x19:			// Interrupt Register
			// A 1 is written into the interrupt flag that we want to disable
			// Get the bits we need to keep, and AND with the current
			mRegInterrupt &= (~pData & 0x0F);
			//board()->interruptClear(this);

			// Check for an active interrupt against the mask
			if( mRegInterrupt & mRegInterruptEnabled)
				mRegInterrupt |= 0x80;				// IRQ Waiting Bit

			break;

		case 0x1A:			// Interrupt Enabled
			mRegInterruptEnabled = (pData & 0x0F);
			// Check for an active interrupt against the mask
			if( mRegInterrupt & mRegInterruptEnabled) {

				mRegInterrupt |= 0x80;				// IRQ Waiting Bit
				cCpu *cpu = mSystem->deviceFind<cCpu>("CPU", true);
				cpu->interruptAdd( new cInterrupt("VIC", this) );

			} else {
				mRegInterrupt &= 0x7F;				// Only keep lower 4 bits (IRQ Waiting Bit off)

				cCpu *cpu = mSystem->deviceFind<cCpu>("CPU", true);
				cpu->interruptRemove( "VIC" );
			}
			break;
			
		case 0x1B:
			mRegMobDataPriority = pData;
			break;

		case 0x1C:
			mRegMobMultiColor = pData;
			break;

		case 0x1D:
			mRegMobXExpansion = pData;
			break;

		case 0x1E:			// MOB on MOB Col
		case 0x1F:			// MOB on DTA Col
			break;

		case 0x20:			// Border Color
			mRegBorderColor = (pData & 0x0F);
			break;
							// Background Colors
		case 0x21:	case 0x22:  case 0x23:  case 0x24:
			pAddress &= 0x07;
			mRegBackgroundColor[pAddress - 1] = (pData & 0x0F);		// Only lower4bits can be used
			break;

		case 0x25:
		case 0x26:
			pAddress &= 0x3;
			mRegMobMultiColors[pAddress - 1] = (pData & 0x0F);
			break;

		case 0x27: case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E:
			pAddress &= 0x7;
			mRegMobColor[pAddress - 1] = (pData & 0x0F);
			break;
	}

}

void cVideo_Mos_8567::decode_StandardText() {
	cSystem_Commodore_64	*system = mSystemGet<cSystem_Commodore_64>();

	// Read char pointer from video
	word data = system->deviceReadByte( this, mVidSrc++ ) << 3;

	// Get memory address in char rom
	mVidChar = mVidBaseChar + data;

	// Read char row
	data = system->deviceReadByte( this, mVidChar + (mDrawY % 8) );
	
	// ColorRam Pointer
	word colorP = ((mRegMemoryPtrs & 0xF0) << 4) + (mDrawY % 8) + mRegRowCounter;

	// The color
	word color = system->deviceReadWord( this, colorP );

	color = (color >> 8) & 0x0f;

	// Lets draw 8 bits
	for( size_t bit = 0; bit < 8; ++bit, ++mBufferPtr ) {
		
		if( data & 0x80 )
			*mBufferPtr = (byte) color;
		else
			*mBufferPtr = mRegBackgroundColor[0];

		data <<= 1;
	}
}
