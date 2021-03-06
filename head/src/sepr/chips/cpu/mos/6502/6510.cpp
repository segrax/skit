#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "6502.hpp"
#include "6510.hpp"

cCpu_Mos_6510::cCpu_Mos_6510( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cCpu_Mos_6502( pName, pSepr, pSystem, pParent ) {

}

void cCpu_Mos_6510::reset() {
	mProcessorPort[0] = 0x2F;
	mProcessorPort[1] = 0x37;

	cCpu_Mos_6502::reset();
}

byte cCpu_Mos_6510::busReadByte( size_t pAddress ) {
	if(pAddress == 0 || pAddress == 1)
		return mProcessorPort[pAddress];

	return 0;
}

void cCpu_Mos_6510::busWriteByte( size_t pAddress, byte pData ) {
	if(pAddress == 0 || pAddress == 1)
		mProcessorPort[pAddress] = pData;
}

void cCpu_Mos_6510::busWriteWordLE( size_t pAddress, word pData ) {

}
