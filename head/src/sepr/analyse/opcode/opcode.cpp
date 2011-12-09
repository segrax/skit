#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/opcode.hpp"

#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "analyse/opcode/opcode.hpp"

cAnalysis_Opcode::cAnalysis_Opcode( cCpu *pCpu, cAnalysis_Opcode *pPrevious ) {
    
    mCpu = pCpu;

    mRegistersAfter = 0;
    mRegistersBefore = mCpu->registersClone();

    mPreviousOpcode = pPrevious;
}

cAnalysis_Opcode::~cAnalysis_Opcode() {

    delete mRegistersBefore;
    delete mRegistersAfter;
}

void cAnalysis_Opcode::ParameterAdd( cAnalysis_Opcode_Parameter *pParameter ) {

    mParameters.push_back( pParameter );
}

void cAnalysis_Opcode::OpcodeComplete() {
    mRegistersAfter = mCpu->registersClone();
}
