#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/opcode.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "systems/system.hpp"

cCpu::cCpu( std::string pName, cSepr *pSepr, cDevice *pParent ) : cDevice( pName, pSepr, pParent ) {

	mOpcodes = 0;
	mOpcodeCount = 0;
	mOpcodeCurrent = 0;
	mOpcodeNumber = 0;

	mOpcode_Reset =  new cChip_Opcode();
	mOpcode_Reset->_OPCODE( cCpu, o_Reset, o_Reset, 8 );

	mOpcode_Unknown = new cChip_Opcode();
	mOpcode_Unknown->_OPCODE( cCpu, o_Unknown_Opcode, o_Unknown_Opcode, 8 );
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
		mSystem()->mDebugGet()->device( eDebug_Message, mSepr, this, debug_CPU_Info_String() );

	if( !mAnalyse )
		return;

	if(mOpcodeCurrent)
		(*mOpcodeCurrent->mOpcodeFunctionAnalyse)();
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

	mSystem()->mDebugGet()->device( eDebug_Message, mSepr, this, msg.str());
}

void cCpu::o_Reset() {

}
