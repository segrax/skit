#include "sepr.hpp"
#include "device/device.hpp"
#include "rom.hpp"
#include "common/file.hpp"

cChip_Rom::cChip_Rom( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cDevice(pName, pSepr, pSystem, pParent ) { 
	mMemory = 0;
	mMemorySize = 0;
}

cChip_Rom::~cChip_Rom() {

	delete mMemory;
}

size_t cChip_Rom::cycle() {

	return 0;
}

bool cChip_Rom::loadFile( std::string pFile ) {

	mMemory = local_FileRead( pFile, mMemorySize );

	return (mMemory == 0 ) ? false : true;
}

byte cChip_Rom::busReadByte( size_t pAddress ) {
	
	return mMemory[ pAddress & (mMemorySize-1)];
}

word cChip_Rom::busReadWordLE( size_t pAddress ) {

	return readWordLE( &mMemory[ pAddress & (mMemorySize-1)] );
}
