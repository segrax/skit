#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "6522.hpp"
#include "systems/system.hpp"

cVia_Mos_6522::cVia_Mos_6522( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cDevice( pName, pSepr, pSystem, pParent ) {

}

byte cVia_Mos_6522::busReadByte( size_t pAddress ) {

	return 0;
}

void cVia_Mos_6522::busWriteByte( size_t pAddress, byte pData ) {


}
