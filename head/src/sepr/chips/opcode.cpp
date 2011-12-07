#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/opcode.hpp"

#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"

cChip_Opcode_Analysis::cChip_Opcode_Analysis( cCpu *pCpu, cChip_Opcode_Analysis *pPrevious ) {
    
    // must copy all cpu registers to mRegisters
    mRegisters = pCpu->registersClone();



}
