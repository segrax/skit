#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "systems/system.hpp"
#include "chips/opcode.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "6502.hpp"

cCpu_Mos_6502::cCpu_Mos_6502( std::string pName, cSepr *pSepr, cDevice *pParent ) : cCpu( pName, pSepr, pParent ) {
	
	mTmpOpcode = 0;

	opcodesPrepare();
	registersPrepare();

	// Create the reset opcode
	mOpcode_Reset->_OPCODE( cCpu_Mos_6502, o_Reset, a_Nop, 8 );
}

cCpu_Mos_6502::~cCpu_Mos_6502() {
	
	delete mOpcode_Reset;

}

void cCpu_Mos_6502::reset() {

	mCycle = 0;
	mCycles = 9;
	mCyclesTotal = 0;

	mTmpOpcode = 0;

	mOpcodeCurrent = mOpcode_Reset;
}

void cCpu_Mos_6502::cycle() {

	// No Instruction, lets fetch one, or use a pre-fetched one
	if( !mOpcodeCurrent ) {
		byte op = mTmpOpcode;

		if( !op )
			op = mSystem()->busReadByte( regPC++ );

		opcodeSet( op );

		cycleNext();

		if(!mTmpOpcode)
			return;

		mTmpOpcode = 0;
	}

	opcodeExecute();
	cycleNext();
}

void cCpu_Mos_6502::registerFlagSet( size_t pData ) {
	if(!pData)
		flagZero = true;
	else
		flagZero = false;

	if(pData & 0x80)
		flagNegative = true;
	else
		flagNegative = false;
}

void cCpu_Mos_6502::stackPush( byte pData )	{	

	mSystem()->busWriteByte( 0x0100 + regSP(), pData); 
	--regSP;
}	

byte cCpu_Mos_6502::stackPop() {
	++regSP;
	byte tmp = mSystem()->busReadByte( 0x0100 + regSP() );
	
	return tmp;
}

void cCpu_Mos_6502::registersPrepare() {

	mRegFL			= new cChip_Register_Flags( this, "Flags");
	
	mFlagNegative	= new cChip_Register_Flag( this, mRegFL,"Negative",	0x80 );
	mFlagOverflow	= new cChip_Register_Flag( this, mRegFL,"Overflow",	0x40 );
	mFlagReserved	= new cChip_Register_Flag( this, mRegFL,"Reserved",	0x20 );
	mFlagBreak		= new cChip_Register_Flag( this, mRegFL,"Break",	0x10 );
	mFlagDecimal	= new cChip_Register_Flag( this, mRegFL,"Decimal",	0x08 );
	mFlagInterrupt	= new cChip_Register_Flag( this, mRegFL,"Interrupt",0x04 );
	mFlagZero		= new cChip_Register_Flag( this, mRegFL,"Zero",		0x02 );
	mFlagCarry		= new cChip_Register_Flag( this, mRegFL,"Carry",	0x01 );

	mRegFL->add( mFlagNegative );
	mRegFL->add( mFlagOverflow );
	mRegFL->add( mFlagReserved );
	mRegFL->add( mFlagBreak );
	mRegFL->add( mFlagDecimal );
	mRegFL->add( mFlagInterrupt );
	mRegFL->add( mFlagZero );
	mRegFL->add( mFlagCarry );

	mRegA		= new cChip_Register_Byte( this,	"A", 0, true);
	mRegX		= new cChip_Register_Byte( this,	"X", 0, true); 
	mRegY		= new cChip_Register_Byte( this,	"Y", 0, true);
	mRegStack	= new cChip_Register_Byte( this,	"Stack", 0, true);
	mRegPC		= new cChip_Register_Word( this,	"PC", 0, false);

	mRegisters.add( mRegA );
	mRegisters.add( mRegX );
	mRegisters.add( mRegY );
	mRegisters.add( mRegStack );
	mRegisters.add( mRegPC );
	mRegisters.add( mRegFL );
}
