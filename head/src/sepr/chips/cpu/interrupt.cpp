#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"

cInterrupt::cInterrupt( std::string pName, cDevice *pDevice ) {
	
	mDevice = pDevice;
	mName = pName;
}
