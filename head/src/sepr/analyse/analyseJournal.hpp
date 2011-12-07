class cChip_Opcode_Analysis;

class cAnalyseJournal {
private:
    std::multimap< size_t, cChip_Opcode_Analysis* >   mJournals;

public:

    void                    journalAdd( size_t pClock, cChip_Opcode_Analysis *pOpcode );
};
