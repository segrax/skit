class cAnalysis_Opcode;

// Block is a set of instructions which occur until a branch or condition is reached, a new block is then created for the
// instructions following this point
class cAnalyseBlock {
private:
    std::vector< cAnalysis_Opcode* >   mOpcodes;

public:

    void                    opcodeAdd( cAnalysis_Opcode *pOpcode );
};
