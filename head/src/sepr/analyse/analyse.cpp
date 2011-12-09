#include "sepr.hpp"

#include "analyse.hpp"

#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"

cAnalyse::cAnalyse( cSepr *pSepr ) {

    mSepr = pSepr;
}

void cAnalyse::opcodeAdd( cAnalysis_Opcode *pOpcode ) {
    
    mJournal.journalAdd( mSepr->mSystemGet()->mCyclesTotalGet(), pOpcode );
}
