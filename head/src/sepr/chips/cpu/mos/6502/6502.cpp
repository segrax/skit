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

	// Create the reset and interrupt opcodes
	mOpcode_Reset->_OPCODE( cCpu_Mos_6502, o_Reset, a_Nop, 8 );
	mOpcode_Interrupt->_OPCODE( cCpu_Mos_6502, o_Interrupt, o_Interrupt, 8);

}

cCpu_Mos_6502::~cCpu_Mos_6502() {
	
	delete mOpcode_Reset;

}

std::string cCpu_Mos_6502::debug_CPU_Info_String() {
	std::stringstream	msg;
	
	if( !mOpcodeCurrent->mOpcodeDescription )
		return "";

	msg << regPC.debug_CPU_Info_String() << " ";
	msg << ": 0x" << std::hex << std::uppercase << mOpcodeNumber;
	
	msg << "       '" << mOpcodeCurrent->mOpcodeDescription << "'";

	msg << "\n";
	msg << regA.debug_CPU_Info_String() << " ";
	msg << regX.debug_CPU_Info_String() << " ";
	msg << regY.debug_CPU_Info_String() << " ";

	msg << regSP.debug_CPU_Info_String() << " ";
	msg << regFL.debug_CPU_Info_String() << " ";

	msg << "\n\n";
	return msg.str();
}

void cCpu_Mos_6502::reset() {

	mCycle = 0;
	mCycles = 9;
	mCyclesTotal = 0;

	mTmpOpcode = 0;

	mOpcodeCurrent = mOpcode_Reset;
}

size_t cCpu_Mos_6502::cycle() {

	// No current opcode, and pending interrupts
	if( !mOpcodeCurrent && !mInterruptCurrent && mInterrupts.size() ) {
		
		cInterrupt_NMI *intNMI = cCpu::interruptNMIFind();
	
		if( intNMI )  {
			mOpcodeCurrent = mOpcode_Interrupt;
			mInterruptCurrent = intNMI;
			mTmpWord = 0xFFFA;
			mCycle = 1;
		} else {
			// Interrupts enabled?
			if( flagInterrupt == false ) {

				cInterrupt *intNorm = cCpu::interruptFind();
				if( intNorm ) {
					mOpcodeCurrent = mOpcode_Interrupt;
					mInterruptCurrent = intNorm;
					mTmpWord = 0xFFFE;
					mCycle = 1;
				}
			}
		}
	}

	// No Instruction, lets fetch one, or use a pre-fetched one
	if( !mOpcodeCurrent ) {
			
		//if(regPC() == 0xea81 )
		//	mDebug = true;

		byte op = mTmpOpcode;

		if( !op )
			op = mSystem()->busReadByte( regPC++ );

		opcodeSet( op );

		cycleNext();

		if(!mTmpOpcode)
			return 1;

		mTmpOpcode = 0;
	}

	opcodeExecute();
	cycleNext();

	return 1;
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
	return mSystem()->busReadByte( 0x0100 + regSP() );
}

void cCpu_Mos_6502::o_Interrupt() {
	CYCLE(1)
		mCycles = 7;

	CYCLE(2)
		stackPush( regPC() );
	
	CYCLE(3)
		stackPush( regPC() >> 8);

	CYCLE(4)
		stackPush( (byte) regFL.value() );
	
	CYCLE(5) 
		regPC = mSystem()->busReadByte( mTmpWord++ );

	CYCLE(6) {
		regPC |= (mSystem()->busReadByte( mTmpWord ) << 8);

		// Disable Interrupts
		flagInterrupt = true;

		interruptRemove(mInterruptCurrent);
		mInterruptCurrent = 0;
	}

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
	mRegStack	= new cChip_Register_Byte( this,	"Stack", 0, false);
	mRegPC		= new cChip_Register_Word( this,	"PC", 0, false);

	mRegisters.add( mRegA );
	mRegisters.add( mRegX );
	mRegisters.add( mRegY );
	mRegisters.add( mRegStack );
	mRegisters.add( mRegPC );
	mRegisters.add( mRegFL );

	// Set the registers to default values
	(*mFlagReserved) = true;
}

void cCpu_Mos_6502::o__Bit() {
	byte		flags	= regFL.value();

	mTmpWord			= mTmpByte & regA();

	// Clear flags
	flags &= ~(flagZERO | flagOVERFLOW | flagNEGATIVE);
	
	flags |= (mTmpByte & (flagOVERFLOW | flagNEGATIVE));

	regFL.valueSet(flags);
	if( mTmpWord )
		flagZero = false;
	else
		flagZero = true;
}

void cCpu_Mos_6502::o__AddWithCarry() {
	byte flags = regFL.value();

	// Decimal Add
	if( flagDecimal == true ) {
		//TODO
		flagCarry = ((mTmpWord > 0xFF ) ? true : false);

		regA = (byte) mTmpWord;
	} else {
	// Binary Add
		mTmpWord = regA();
		mTmpWord += mTmpByte + flagCarry.getInt();

		// Turn off all flags which are modified
		flags &= ~ (flagNEGATIVE | flagOVERFLOW | flagZERO | flagCARRY);
		
		flags |= (~(regA() ^ mTmpByte) & (regA() ^ (mTmpWord & 0xFF)) & 0x80 ? flagOVERFLOW : 0);

		regFL.valueSet( flags );
		regA = (byte) mTmpWord;

		flagCarry = ((mTmpWord > 0xFF ) ? true : false);
	}

}

void cCpu_Mos_6502::o__SubWithCarry() {
	byte flags = regFL.value();
	
	if( flagDecimal == true ) {
		// Decimal Add
		// TODO

	} else {
		// Binary Add
		mTmpWord = regA();
		mTmpWord -= mTmpByte + ( !flagCarry.getInt() );
		
		flags &= ~ (flagNEGATIVE | flagOVERFLOW | flagZERO | flagCARRY);
		flags |= ((regA() ^ mTmpByte) & (regA() ^ (mTmpWord & 0xFF)) & 0x80 ? flagOVERFLOW : 0);
		
		regFL.valueSet( flags );
		regA = (byte) mTmpWord;

		flagCarry = ((mTmpWord < 0x100 ) ? true : false);
	}
}
