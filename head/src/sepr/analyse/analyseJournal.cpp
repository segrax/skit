#include "sepr.hpp"

#include "analyseJournal.hpp"
#include "analyse.hpp"


void cAnalyseJournal::journalAdd( size_t pClock, cChip_Opcode_Analysis *pOpcode ) {

    mJournals.insert( std::make_pair( pClock, pOpcode ) );
}
