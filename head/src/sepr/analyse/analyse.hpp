class cAnalyseJournal;
class cChip_Opcode_Analysis;
class cSepr;

class cAnalyse {
private:
    cSepr               *mSepr;
    cAnalyseJournal      mJournal;

public:
                         cAnalyse(  cSepr *pSepr  );

    void                 opcodeAdd( cChip_Opcode_Analysis *pOpcode );
};
