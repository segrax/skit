class cChip_Opcode;

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
						 cCpu( std::string pName, cSepr *pSepr, cDevice *pParent );
						~cCpu();
	
	virtual void		 interruptAdd( cInterrupt *pInterrupt );
	virtual void		 interruptRemove( cInterrupt *pInterrupt );
	virtual void		 interruptRemove( std::string pName );

	virtual		void	 registerFlagSet( size_t pData ) = 0;

	const cChip_Opcode	*mOpcodeCurrentGet() { return mOpcodeCurrent; }

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

protected:

	virtual void		 o_Reset();
	virtual void		 o_Unknown_Opcode();
	virtual void		 o_Interrupt();

};
