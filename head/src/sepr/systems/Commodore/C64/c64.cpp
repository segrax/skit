#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/memory/rom.hpp"
#include "chips/memory/ram.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "chips/cpu/mos/6502/6502.hpp"
#include "chips/cpu/mos/6502/6510.hpp"
#include "chips/interfaceAdapter/mos/6526.hpp"
#include "input/keyboard/keyboard.hpp"

#include "systems/system.hpp"
#include "chips/video/video.hpp"
#include "chips/video/mos/8567.hpp"
#include "skit/video/window.hpp"

#include "c64_keyboard.hpp"
#include "c64.hpp"

cSystem_Commodore_64::cSystem_Commodore_64( cSepr *pSepr ) : cSystem("Commodore64", pSepr) {
	
	mBasic	= new cChip_Rom("BASIC", pSepr, this );
	mKernal = new cChip_Rom("KERNAL", pSepr, this );
	mChar	= new cChip_Rom("CHAR", pSepr, this );

	mRam = new cChip_Ram("RAM", pSepr, this, 0x10000);

	mCpu = new cCpu_Mos_6510("CPU", pSepr, this );
	mVideo = new cVideo_Mos_8567("VIDEO", pSepr, this );

	mRamColor = new cChip_Ram("COLOR", pSepr, this, 0x400);

	//mCia1 = new cCia_Mos_6526("CIA1", pSepr, this );
	mCia2 = new cCia_Mos_6526("CIA2", pSepr, this );

	mKeyboard = new cCommodore_64_Keyboard("KEYBOARD", pSepr, this );
	mCia1 = mKeyboard;

	mWindow = new cVideoWindow( 403, 284, 1, false );

	mVideo->windowSet( mWindow );
}

cSystem_Commodore_64::~cSystem_Commodore_64() {

	delete mWindow;
}

bool cSystem_Commodore_64::prepare() {

	if(!mBasic->loadFile( systemDataPath( "BASIC.ROM" ) ))
		return false;

	if(!mKernal->loadFile( systemDataPath( "KERNAL.ROM" ) ))
		return false;

	if(!mChar->loadFile( systemDataPath( "CHAR.ROM") ))
		return false;

	deviceConnect( mBasic,		0xA000, 0x2000 );
	deviceConnect( mChar,		0xD000, 0x1000 );
	deviceConnect( mKernal,		0xE000, 0x2000 );

	deviceConnect( mCpu,		0x0000, 0x0002 );
	deviceConnect( mRam,		0x0000, 0x10000 );
	deviceConnect( mRamColor,	0xD800, 0x400 );

	deviceConnect( mVideo,		0xD000, 0x0400 );
	deviceConnect( mCia1,		0xDC00, 0x100 );
	deviceConnect( mCia2,		0xDD00, 0x100 );

	mCpu->reset();
	//mCpu->threadStart();

	mVideo->reset();
	//mVideo->threadStart();

	mCia1->reset();
	mCia2->reset();
	// Force debug mode if compiled as debug build
#ifdef _DEBUG
	mCpu->mDebugSet(true);
#endif

	return true;
}

cDevice *cSystem_Commodore_64::busVicGet(	size_t pAddress, bool pRead ) {
		word	address = pAddress; // | CIAPins	// Need to read pin14/15 from the CIA

		if((address & 0x7000) == 0x1000) {
			return mChar;
		}

		return mRam;
	}

cDevice *cSystem_Commodore_64::deviceIOGet( size_t pAddress, bool pRead ) {
	// VIC-II
	if( pAddress >= 0xD000 && pAddress <= 0xD3FF )
		return mVideo;

	// SID
	if( pAddress >= 0xD400 && pAddress <= 0xD7FF )
		;

	// Color Ram
	if( pAddress >= 0xD800 && pAddress <= 0xDBFF )
		return mRamColor;

	// CIA 1
	if( pAddress >= 0xDC00 && pAddress <= 0xDCFF )
		return mCia1;

	// CIA 2
	if( pAddress >= 0xDD00 && pAddress <= 0xDDFF )
		return mCia2;

	// IO Area 1
	if( pAddress >= 0xDE00 && pAddress <= 0xDEFF )
		;

	// IO Area 2
	if( pAddress >= 0xDF00 && pAddress <= 0xDFFF )
		;

	return mRam;
}

cDevice	*cSystem_Commodore_64::deviceGet( size_t pAddress, bool pRead ) {
	byte	 memoryConfig	= (mCpu->mProcessorPortGet(1) & 0x07);

	// Ram visible in all areas, or writing
	if( !memoryConfig ) 
		return mRam;

	// Reading basic
	if( pAddress >= 0xA000 && pAddress <= 0xBFFF) {
		if( memoryConfig & 0x03 )
			return mBasic;
	}
		
	// Reading CharRom / IO Area
	if( pAddress >= 0xD000 && pAddress <= 0xDFFF ) {

		// I/O area, but not if bit3 is the only one enabled
		if( (memoryConfig & 0x04) && (memoryConfig != 0x04) ) {
			return deviceIOGet( pAddress, pRead );
		}

		// Read Char Rom, but only if bit3 is not enabled, and bit1 or 2 is
		if( (!(memoryConfig & 0x04)) && (memoryConfig & 0x03)) {
			return mChar;
		}
	}

	// Reading kernel
	if( pAddress >= 0xE000 && pAddress <= 0xFFFF) {
		if( memoryConfig & 0x02)
			return mKernal;
	}

	if( pAddress >= 0 && pAddress <= 1 )
		return mCpu;

	return mRam;
}

size_t cSystem_Commodore_64::cycle() {
	std::vector<SDL_Event>::iterator eventIT;

	pthread_mutex_lock( &mEventQueue );
	for( eventIT = mEvents.begin(); eventIT != mEvents.end(); ++eventIT ) {
		
		if( eventIT->type == SDL_KEYDOWN )
			mKeyboard->sdlEvent( (SDL_KeyboardEvent*) &(*eventIT) );

		else if( eventIT->type == SDL_KEYUP )
			mKeyboard->sdlEvent( (SDL_KeyboardEvent*) &(*eventIT) );
	}
	mEvents.clear();
	pthread_mutex_unlock( &mEventQueue );

	/*while( mCpu->mCyclesRemainingGet() != 0 || mVideo->mCyclesRemainingGet() != 0 ) {
		Sleep(mSleepTime);
	}

	mVideo->mCyclesRemainingAdd(1);
	mCpu->mCyclesRemainingAdd(1);*/


	mVideo->cycle();
	mCpu->cycle();
	mCia1->cycle();
	mCia2->cycle();

	return 1;
}

word cSystem_Commodore_64::deviceReadWord( cVideo_Mos_8567 *pVic, size_t pAddress ) {
	word ret = busVicGet( pAddress, true )->busReadByte( pAddress );

	byte color = mRamColor->busReadByte( pAddress & 0x3FF );

	ret &= 0xF0;
	ret |= (color << 8) & 0xF00;

	return ret;
}

byte cSystem_Commodore_64::deviceReadByte( cDevice *pFromDevice, size_t pAddress ) {

	if( pFromDevice->mNameGet() == "VIDEO" ) {
		return busVicGet( pAddress, true )->busReadByte( pAddress );
	}

	return 0;
}

byte cSystem_Commodore_64::busReadByte( size_t pAddress ) {
	cDevice *device = deviceGet( pAddress, true );

	return device->busReadByte( pAddress );
}

word cSystem_Commodore_64::busReadWordLE( size_t pAddress ) {
	cDevice *device = deviceGet( pAddress, true );

	return device->busReadWordLE( pAddress );
}

void cSystem_Commodore_64::busWriteByte( size_t pAddress, byte pData ) {
	cDevice *device = deviceGet( pAddress, false );

	device->busWriteByte( pAddress, pData );
}

void cSystem_Commodore_64::busWriteWordLE( size_t pAddress, word pData ) {
	cDevice *device = deviceGet( pAddress, false );

	device->busWriteWordLE( pAddress, pData );
}

SDL_Surface	*cSystem_Commodore_64::videoGet() {

	return mVideo->surfaceGet();
}

void cSystem_Commodore_64::interruptAdd( std::string pName, cDevice *pDevice ) {
	cInterrupt *interrupt = 0;

	if( mCpu->interruptFind(pName ) )
		return;

	// Create NMIs for certain devices
	if( pDevice == mCia2 ) {
		interrupt = new cInterrupt_NMI( pName, pDevice );
	
	} else
		interrupt = new cInterrupt( pName, pDevice);

	mCpu->interruptAdd( interrupt );
}

void cSystem_Commodore_64::interruptRemove( std::string pName ) {

	mCpu->interruptRemove(pName);
}
