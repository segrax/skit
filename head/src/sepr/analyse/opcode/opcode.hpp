class cChip_Registers;
class cCpu;
class cAnalysis_Opcode_Parameter;


class cAnalysis_Opcode {

private:
    cCpu                                    *mCpu;
    cChip_Registers                         *mRegistersBefore, *mRegistersAfter;    // CPU Registers before we start

    std::vector< cAnalysis_Opcode_Parameter* >   mParameters;                       // Parameters used by the instruction
    cAnalysis_Opcode                            *mPreviousOpcode;

public:
                                             cAnalysis_Opcode( cCpu *pCpu, cAnalysis_Opcode *pPrevious );
                                            ~cAnalysis_Opcode();

    void                                     ParameterAdd( cAnalysis_Opcode_Parameter *pParameter );
    void                                     OpcodeComplete();                      // Opcode has completed execution

};
