#include "sepr.hpp"

#include "analyseJournal.hpp"
#include "analyse.hpp"

#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"

cAnalyse::cAnalyse( cSepr *pSepr ) {

    mSepr = pSepr;
}

void cAnalyse::opcodeAdd( cChip_Opcode_Analysis *pOpcode ) {
    
    mJournal.journalAdd( mSepr->mSystemGet()->mCycleGet(), pOpcode );
}
