
// Base Function call
template<class tResult>
struct cFunctionCall_Null {
	virtual ~cFunctionCall_Null() {}

	virtual bool isValid() const = 0;
	virtual tResult operator()() const = 0;
};

// Function call with no parameters
template<class tResult, class tClass>
class cFunctionCall_NoParams : public cFunctionCall_Null<tResult> {
public:
	typedef tResult (tClass::*tFuncCall)();

	cFunctionCall_NoParams(tClass *pClass, const tFuncCall &pFunction) : mClass(pClass), mFunction(pFunction) {}

	bool isValid() const { return mFunction != 0 && mClass != 0; }
	tResult operator()() const {
		return (mClass->*mFunction)();
	}
private:
	mutable tClass *mClass;
	const tFuncCall mFunction;
};


typedef cFunctionCall_Null<void> tFunctionCall;

// Opcode
class cChip_Opcode {
public:
	tFunctionCall	*mOpcodeFunction, *mOpcodeFunctionAnalyse;
	const char		*mOpcodeDescription;
	size_t			 mCycles;

public:
	cChip_Opcode() : mOpcodeFunction(0), mOpcodeFunctionAnalyse(0), mOpcodeDescription(0), mCycles(0) {}
	~cChip_Opcode() {
		setProc(0, 0, 0, 0);
	}

	void setProc(tFunctionCall *pOpcodeFunction, tFunctionCall *pOpcodeFunctionAnalysis, const char *d, size_t pCycles) {
		if( mOpcodeFunctionAnalyse != pOpcodeFunctionAnalysis ) {
			delete mOpcodeFunctionAnalyse;
			mOpcodeFunctionAnalyse = pOpcodeFunctionAnalysis;
		}

		if (mOpcodeFunction != pOpcodeFunction) {
			delete mOpcodeFunction;
			mOpcodeFunction = pOpcodeFunction;
		}

		mOpcodeDescription = d;
		mCycles = pCycles;
	}
}; 

class cChip_Registers;
class cCpu;

class cChip_Opcode_Analysis {
private:
    cChip_Registers     *mRegisters;

    
public:
                         cChip_Opcode_Analysis( cCpu *pCpu, cChip_Opcode_Analysis *pPrevious );

};

#define _OPCODE(ver, o, a, c) setProc(	new cFunctionCall_NoParams<void, ver>(this, &ver::o), new cFunctionCall_NoParams<void, ver>(this, &ver::a), #o, c) 
