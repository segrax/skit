class cChip_Opcode;
class cChip_Opcode_Analysis;

#include "sepr/chips/cpu/interrupt.hpp"

#define CYCLE(i)	if(mCycle == i)

class cCpu : public cDevice {
private:

protected:
	std::multimap< std::string, cInterrupt* >	 mInterrupts;
	
	size_t				 mOpcodeCount, mOpcodeNumber;
	cChip_Opcode		*mOpcodes;								// Opcode Table
	cChip_Opcode		*mOpcodeCurrent;						//

	cChip_Opcode		*mOpcode_Unknown,		*mOpcode_Reset;	// Handlers for reset and unknown
	cChip_Opcode		*mOpcode_Interrupt;
    
    cChip_Opcode_Analysis *mOpcodeAnalysis;

    cChip_Registers      mRegistersCycleStart;
	cChip_Registers		 mRegisters;

	cInterrupt			*mInterruptCurrent;

protected:
	virtual void		 cycleNext();
	virtual size_t		 cycle() = 0;

	virtual std::string	 debug_CPU_Info_String() = 0;

	virtual void		 opcodesPrepare() = 0;
	void				 opcodeSet( size_t pOp );
	void				 opcodeAnalyse();
	void				 opcodeExecute();

	virtual 	void	 registersPrepare() = 0;

public:
						 cCpu( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent );
						~cCpu();
	
	virtual void		 interruptAdd( cInterrupt *pInterrupt );
	virtual void		 interruptRemove( cInterrupt *pInterrupt );
	virtual void		 interruptRemove( std::string pName );

    virtual cChip_Registers *registersClone() = 0;
	virtual	void	         registerFlagSet( size_t pData ) = 0;
        
	inline const cChip_Opcode	*mOpcodeCurrentGet() { return mOpcodeCurrent; }

    virtual cInterrupt_NMI	*interruptNMIFind( ) {
		std::multimap< std::string, cInterrupt* >::iterator	intIT;

		for( intIT = mInterrupts.begin(); intIT != mInterrupts.end(); ++intIT ) {
			if( typeid( *intIT->second ) == typeid( cInterrupt_NMI ) )
				return (cInterrupt_NMI*) intIT->second;

		}

		return 0;
	}

	virtual cInterrupt	*interruptFind( ) {
		std::multimap< std::string, cInterrupt* >::iterator	intIT;

		for( intIT = mInterrupts.begin(); intIT != mInterrupts.end(); ++intIT ) {
			if( typeid( *intIT->second ) == typeid( cInterrupt ) )
				return intIT->second;

		}

		return 0;
	}

	virtual cInterrupt *interruptFind( std::string pName ) {
		std::multimap< std::string, cInterrupt* >::iterator	intIT;

		for( intIT = mInterrupts.begin(); intIT != mInterrupts.end(); ++intIT ) {
			if( intIT->second->mNameGet() == pName )
				return intIT->second;

		}

		return 0;
	}

    virtual cChip_Register* registerGet( std::string pName ) {
        return mRegisters.get( pName );
    }

    virtual cChip_Registers *registersGet() {
        return &mRegisters; 
    }

protected:

	virtual void		 o_Reset();
	virtual void		 o_Unknown_Opcode();
	virtual void		 o_Interrupt();
    
    virtual void a_Reset();
	virtual void a_Unknown_Opcode();
	virtual void a_Interrupt();
};
