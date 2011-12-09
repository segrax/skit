class cAnalysis_Opcode;

class cAnalyseJournal {
private:
    std::multimap< size_t, cAnalysis_Opcode* >   mJournals;

public:

    void                    journalAdd( size_t pClock, cAnalysis_Opcode *pOpcode );
};
