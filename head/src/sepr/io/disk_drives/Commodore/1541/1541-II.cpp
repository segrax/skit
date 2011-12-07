#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/memory/rom.hpp"
#include "chips/memory/ram.hpp"
#include "chips/register.hpp"
#include "chips/opcode.hpp"
#include "chips/cpu/cpu.hpp"
#include "chips/cpu/mos/6502/6502.hpp"
#include "systems/system.hpp"
#include "io/port/port.hpp"
#include "io/drive.hpp"
#include "io/disk_drives/disk_drive.hpp"
#include "chips/interfaceAdapter/mos/6526.hpp"
#include "1541-II.hpp"


cDrive_Commodore_1541_II::cDrive_Commodore_1541_II( std::string pName, cSepr *pSepr ) : cDisk_Drive( pName, pSepr ) {
	
	mRom = new cChip_Rom("1541_ROM", pSepr, this, this );
	mRam  = new cChip_Ram("1541_RAM", pSepr, this, this, 0x800);
	mCpu  = new cCpu_Mos_6502("1541_CPU", pSepr, this, this );

	mVia1 = new cVia1( pSepr, this, this );
	mVia2 = new cVia2( pSepr, this, this );
}

void cDrive_Commodore_1541_II::reset() {

}

size_t cDrive_Commodore_1541_II::cycle()  {
    //cChip_Register_Word *PC = (cChip_Register_Word*) mCpu->registerGet("PC");

    // Temp break
    //if( (*PC)() == 0xEC00 )
    //    return 1;

	mCpu->cycle();
	mVia1->cycle();
	mVia2->cycle();

	return 1;
}

bool cDrive_Commodore_1541_II::prepare() {
	if(!mRom->loadFile( systemDataPath("1541ii.rom") ))
		return false;

	deviceConnect( mRam, 0, 0x800 );		// Main Ram
	deviceConnect( mRam, 0x800, 0x1000 );	// Mirror
	deviceConnect( mVia1, 0x1800, 0x0F);
	deviceConnect( mVia2, 0x1C00, 0x0F);
	deviceConnect( mRom, 0xC000, 0x4000 );	// Firmware
	deviceConnect( mCpu, 0, 0 );

	mCpu->reset();
	return true;
}

cDevice	*cDrive_Commodore_1541_II::deviceGet( size_t pAddress, bool pRead ) {

	return cDevice::deviceGet(pAddress, false);
}

byte cDrive_Commodore_1541_II::busReadByte( size_t pAddress ) {
	cDevice *device = cDevice::deviceGet(pAddress, true);

	return device->busReadByte( pAddress );
}

word cDrive_Commodore_1541_II::busReadWordLE( size_t pAddress ) {
	return 0;
}

void cDrive_Commodore_1541_II::busWriteByte( size_t pAddress, byte pData ) {
	cDevice *device = deviceGet(pAddress, false);

	device->busWriteByte( pAddress, pData );
}

void cDrive_Commodore_1541_II::busWriteWordLE( size_t pAddress, word pData ) {
	return;
}
	
void cDrive_Commodore_1541_II::interruptAdd( std::string pName, cDevice *pDevice ) {
	cInterrupt *interrupt = 0;

	if( mCpu->interruptFind( pName ) )
		return;

	interrupt = new cInterrupt( pName, pDevice);

	mCpu->interruptAdd( interrupt );
}

void cDrive_Commodore_1541_II::interruptRemove( std::string pName ) {

    mCpu->interruptRemove(pName);
}

void cDrive_Commodore_1541_II::headMove(int pDirection) {

}

void cDrive_Commodore_1541_II::diskRotateStart() {

}

void cDrive_Commodore_1541_II::diskRotateStop() {

}
