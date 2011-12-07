#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/opcode.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "systems/system.hpp"

#include "../../analyse/analyseJournal.hpp"
#include "../../analyse/analyse.hpp"

cCpu::cCpu( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cDevice( pName, pSepr, pSystem, pParent ) {

	mInterruptCurrent = 0;

	mOpcodes = 0;
	mOpcodeCount = 0;
	mOpcodeCurrent = 0;
	mOpcodeNumber = 0;

    mOpcode_Analysis = 0;

	mOpcode_Reset =  new cChip_Opcode();
	mOpcode_Reset->_OPCODE( cCpu, o_Reset, a_Reset, 8 );

	mOpcode_Unknown = new cChip_Opcode();
	mOpcode_Unknown->_OPCODE( cCpu, o_Unknown_Opcode, a_Unknown_Opcode, 8 );

	mOpcode_Interrupt = new cChip_Opcode();
	mOpcode_Interrupt->_OPCODE( cCpu, o_Interrupt, a_Interrupt, 8 );

}

cCpu::~cCpu() {

	delete[] mOpcodes;
}

void cCpu::opcodeSet( size_t pOp ) {
	mOpcodeNumber = pOp;

	// TODO: Call Debugger
	if(pOp >= mOpcodeCount)
		return;

	mOpcodeCurrent = &mOpcodes[ pOp ];
	mCycles = mOpcodeCurrent->mCycles;
	mCycle = 0;

	opcodeAnalyse();

	// Not implemented opcode?
	if( !mOpcodeCurrent->mOpcodeFunction ) {
		mOpcodeCurrent = mOpcode_Unknown;
		mCycles = 2;
	}
}

void cCpu::opcodeExecute() {

	if(mOpcodeCurrent)
		(*mOpcodeCurrent->mOpcodeFunction)();
}

void cCpu::opcodeAnalyse() {
	if( mDebug )
		mSystem->mDebugGet()->device( mDebugLevel, mSepr, this, debug_CPU_Info_String() );

	if( !mAnalyse )
		return;

    mOpcode_Analysis = new cChip_Opcode_Analysis( this, mOpcode_Analysis );

	if(mOpcodeCurrent)
		(*mOpcodeCurrent->mOpcodeFunctionAnalyse)();

    // Add opcode to journal
    mSystem->mAnalyseGet()->opcodeAdd( mOpcode_Analysis );
}

void cCpu::cycleNext()  {
	++mCycle;

	if(mCycle >= mCycles)
		mOpcodeCurrent = 0;
}

void cCpu::o_Unknown_Opcode() {
	std::stringstream msg;

	msg << debug_CPU_Info_String();
	msg << "\n";
	msg << "Unknown Opcode: 0x";
	msg << std::hex << mOpcodeNumber;
	msg << std::endl;

	mSystem->mDebugGet()->device( eDebug_Message, mSepr, this, msg.str());
}

void cCpu::a_Unknown_Opcode() {

}

void cCpu::o_Reset() {

}

void cCpu::a_Reset() {

}

void cCpu::o_Interrupt() {

}


void cCpu::a_Interrupt() {

}

void cCpu::interruptAdd( cInterrupt *pInterrupt ) {
	
	mInterrupts.insert( std::make_pair( pInterrupt->mNameGet(), pInterrupt ) );
}

void cCpu::interruptRemove( cInterrupt *pInterrupt ) {
	std::multimap< std::string, cInterrupt* >::iterator	intIT;

	for( intIT = mInterrupts.begin(); intIT != mInterrupts.end(); ++intIT ) {
		if( intIT->second == pInterrupt ) {
			delete intIT->second;
			mInterrupts.erase( intIT );
			return;
		}
	}
}

void cCpu::interruptRemove( std::string pName ) {
	std::multimap< std::string, cInterrupt* >::iterator	intIT;

	for( intIT = mInterrupts.begin(); intIT != mInterrupts.end(); ++intIT ) {
		if( intIT->first == pName ) {
			delete intIT->second;
			mInterrupts.erase( intIT );
			return;
		}
	}
}
