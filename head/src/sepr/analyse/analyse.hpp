class cAnalyseJournal;
class cAnalysis_Opcode;
class cSepr;

#include "analyse/opcode/opcode.hpp"
#include "analyse/opcode/parameter.hpp"
#include "analyse/Journal.hpp"
#include "analyse/Block.hpp"

class cAnalyse {
private:
    cSepr               *mSepr;
    cAnalyseJournal      mJournal;

public:
                         cAnalyse(  cSepr *pSepr  );

    void                 opcodeAdd( cAnalysis_Opcode *pOpcode );
};
