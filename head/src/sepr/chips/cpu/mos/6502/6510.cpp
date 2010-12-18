#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "6502.hpp"
#include "6510.hpp"

cCpu_Mos_6510::cCpu_Mos_6510( std::string pName, cSepr *pSepr, cDevice *pParent ) : cCpu_Mos_6502( pName, pSepr, pParent ) {

}

void cCpu_Mos_6510::reset() {
	mProcessorPort = 0x37;

	cCpu_Mos_6502::reset();
}
