#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "6522.hpp"
#include "systems/system.hpp"

cVia_Mos_6522::cVia_Mos_6522( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cDevice( pName, pSepr, pSystem, pParent ) {

}

size_t cVia_Mos_6522::cycle() {

	return 1;
}
