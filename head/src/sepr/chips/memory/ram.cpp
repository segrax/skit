#include "sepr.hpp"
#include "device/device.hpp"
#include "rom.hpp"
#include "ram.hpp"

cChip_Ram::cChip_Ram( std::string pName, cSepr *pSepr, cDevice *pParent, size_t pSize ) : cChip_Rom( pName, pSepr, pParent ) {
	
	mMemory = new byte[ pSize ];
	mMemorySize = pSize;

	memset( mMemory, 0, pSize );
}

void cChip_Ram::busWriteByte( size_t pAddress, byte pData ) {

	mMemory[ pAddress & (mMemorySize-1)] = pData;
}

void cChip_Ram::busWriteWordLE( size_t pAddress, word pData ) {
	 
	writeWordLE( &mMemory[ pAddress & (mMemorySize-1)], pData );

}