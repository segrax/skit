class cAnalysis_Opcode_Parameter {
private:
         

public:

};

class cAnalysis_Opcode_Parameter_Value_Byte : public cBase_Byte, public cAnalysis_Opcode_Parameter {

public:
                        cAnalysis_Opcode_Parameter_Value_Byte( byte pValue ) : cBase_Byte( pValue ) {

                        }
};

class cAnalysis_Opcode_Parameter_Value_Word : public cBase_Word, public cAnalysis_Opcode_Parameter {
                        
public:
                        cAnalysis_Opcode_Parameter_Value_Word( word pValue ) : cBase_Word( pValue ) {

                        }
};

class cAnalysis_Opcode_Parameter_Register : public cAnalysis_Opcode_Parameter {
private:

    std::string          mName;
};

class cAnalysis_Opcode_Parameter_Pointer : public cAnalysis_Opcode_Parameter {

};
