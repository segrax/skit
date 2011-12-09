#include "sepr.hpp"

#include "analyse.hpp"


void cAnalyseJournal::journalAdd( size_t pClock, cAnalysis_Opcode *pOpcode ) {

    mJournals.insert( std::make_pair( pClock, pOpcode ) );
}
