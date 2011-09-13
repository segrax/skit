#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "io/port/port.hpp"
#include "chips/interfaceAdapter/mos/6526.hpp"
#include "chips/interfaceAdapter/mos/6522.hpp"
#include "io/disk_drives/Commodore/1541/via1.hpp"

#include "c64_serial.hpp"

cCommodore_64_Serial::cCommodore_64_Serial( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent, cVia1_SerialPort *pDevice ) : cPort( pName, pSepr, pSystem, pParent ) {
    
    mConnectedDevice = pDevice;
    mDataDirection = 0;

    valueSet(0);
}

void cCommodore_64_Serial::busWriteByte( dword pAddress, byte  pData ) {
    switch( pAddress ) {
        case 0:
            valueSet( pData );

            mConnectedDevice->pinsCiaUpdate( pData, mDataDirection );
            return;

        case 1:
            mDataDirection = pData;
            return;
    }

}

byte cCommodore_64_Serial::busReadByte( dword		pAddress ) {
    switch( pAddress ) {
    case 0: {
        byte result = valueGet();

        result &= 0x3F;
        result |= (mConnectedDevice->lineClock() ? 0x40 : 0x00);
        result |= (mConnectedDevice->lineData() ? 0x80 : 0x00);
               
        return result;
            }
    case 1:

        return mDataDirection;
    }
}
