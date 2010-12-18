#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/opcode.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"
#include "6502.hpp"
#include "systems/system.hpp"

#define CYCLE(i)	if(mCycle == i)
#define OPCODE(i, o, a, c) mOpcodes[i]._OPCODE(cCpu_Mos_6502, o, a, c) 

void cCpu_Mos_6502::opcodesPrepare() {
	mOpcodeCount = 0x100;

	// Cleanup
	delete[]	mOpcodes;

	// Create the opcode table
	mOpcodes = new cChip_Opcode[0x100];

	// Link the opcodes to the opcode/analysis functions
	OPCODE(0x20,	o_Jump_Subroutine,				a_Jump_Subroutine,				6);
	
	OPCODE(0x60,	o_Return_From_Subroutine,		a_Return_From_Subroutine,		6);
	
	OPCODE(0x78,	o_Flag_Interrupt_Disable_Set,	a_Flag_Interrupt_Disable_Set,	2);
	
	OPCODE(0x85,	o_Store_Accumulator_ZeroPage,	a_Store_Accumulator_ZeroPage,	3);
	OPCODE(0x8D,	o_Store_Accumulator_Absolute,	a_Store_Accumulator_Absolute,	4);
	OPCODE(0x8E,	o_Store_Index_X_Absolute,		a_Store_Index_X_Absolute,		4);
	
	OPCODE(0x9A,	o_Transfer_X_to_StackPtr,		a_Transfer_X_to_StackPtr,		2);
	
	OPCODE(0xA2,	o_Load_Index_X_Immediate,		a_Load_Index_X_Immediate,		2);
	OPCODE(0xA9,	o_Load_Accumulator_Immediate,	a_Load_Accumulator_Immediate,	2);
	
	OPCODE(0xBD,	o_Load_A_Absolute_X,			a_Load_A_Absolute_X,			4);

	OPCODE(0xCA,	o_Decrease_X,					a_Decrease_X,					2);

	OPCODE(0xD0,	o_Branch_Not_Equal,				a_Branch_Not_Equal,				2);
	OPCODE(0xD8,	o_Flag_Decimal_Clear,			a_Flag_Decimal_Clear,			2);
	OPCODE(0xDD,	o_Compare_Absolute_X,			a_Compare_Absolute_X,			4);

}

void cCpu_Mos_6502::o_Unknown_Opcode() {
	std::stringstream msg;

	msg << debug_CPU_Info_String();
	msg << "\n" << "Unknown Opcode";

	mSystem()->mDebugGet()->device( eDebug_Stop, mSepr, this, msg.str());

	mSystem()->mQuitThreadSet();
}

void cCpu_Mos_6502::o_Reset() {
	CYCLE(0)
		regSP = 0;

	// DataBus is set to READ, instead of WRITE... meaning while these exist, they don't write any data
	CYCLE(3)
		--regSP;
	//	stackPush( mRegPC >> 8 );

	CYCLE(4)
		--regSP;
	//	stackPush( mRegPC );

	CYCLE(5)
		--regSP;
	//	stackPush( mRegFlags | 0x10 );

	CYCLE(6)
		regPC = mSystem()->busReadByte( 0xFFFC );

	CYCLE(7)
		regPC |= (mSystem()->busReadByte( 0xFFFD ) << 8);

	CYCLE(8)
		mTmpOpcode = mSystem()->busReadByte( regPC++ );
}

void cCpu_Mos_6502::o_Nop() {

}

// 20: Jump Subroutine
void cCpu_Mos_6502::o_Jump_Subroutine() {
	
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	//CYCLE(2)
	//	;

	CYCLE(3)
		stackPush( regPC() );

	CYCLE(4)
		stackPush( (regPC()) >> 8 );

	CYCLE(5)
		regPC = mTmpByte | (mSystem()->busReadByte( regPC() ) << 8);
}

// 60: Return From Subroutine
void cCpu_Mos_6502::o_Return_From_Subroutine() {
	CYCLE(1)
		;
	CYCLE(2)
		;
	CYCLE(3)
		mTmpWord = (stackPop() << 8);
	CYCLE(4)
		mTmpWord |= stackPop();

	CYCLE(5)
		regPC = mTmpWord + 1;
}

// 78: Disable Interrupt Flag
void cCpu_Mos_6502::o_Flag_Interrupt_Disable_Set() {

	CYCLE(1)
		flagInterrupt = true;
}

// 85: 
void cCpu_Mos_6502::o_Store_Accumulator_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mSystem()->busWriteByte( mTmpByte, regA() );

}

// 8D:
void cCpu_Mos_6502::o_Store_Accumulator_Absolute() {

	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mSystem()->busWriteByte( mTmpWord, regA() );
}

// 8E: Store X Absolute
void cCpu_Mos_6502::o_Store_Index_X_Absolute() {

	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mSystem()->busWriteByte( mTmpWord, regX() );

}

// 9A: Transfer X to StackPtr
void cCpu_Mos_6502::o_Transfer_X_to_StackPtr() {
	
	CYCLE(1)
		regSP = regX();
}

// A2: Load X Register with Immediate Value
void cCpu_Mos_6502::o_Load_Index_X_Immediate() {

	CYCLE(1)
		regX = mSystem()->busReadByte( regPC++ );
}

// A9: Load Accumulator with Immediate Value
void cCpu_Mos_6502::o_Load_Accumulator_Immediate() {

	CYCLE(1)
		regA = mSystem()->busReadByte( regPC++ );
}

// BD: Load A from Absolute_X
void cCpu_Mos_6502::o_Load_A_Absolute_X() {

	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2) {
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);
		if(flagCarry.get())
			mTmpWord += 0x100;
	}

	CYCLE(3) {
		byte highbyte = mTmpWord >> 8;
		mTmpWord += regX();

		// Page Crossed?
		if( highbyte != (mTmpWord >> 8) )
			++mCycles;
		else {
			mCycle = 4;
		}
	}

	CYCLE(4) {
		regA = mSystem()->busReadByte( mTmpWord );

		if(mCycles==4)
			mTmpOpcode = mSystem()->busReadByte( regPC++ );
	}

}

// CA: 
void cCpu_Mos_6502::o_Decrease_X() {

	CYCLE(1)
		--regX;

}

// D0: Branch not Equal
void cCpu_Mos_6502::o_Branch_Not_Equal() {

	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		if( flagZero == false )
			++mCycles;
	}

	CYCLE(2) {
		mTmpWord = regPC();

		byte highbyte = mTmpWord >> 8;
		mTmpWord += (char) mTmpByte;

		// Page Crossed?
		if( highbyte != (mTmpWord >> 8) )
			++mCycles;
		else
			mCycle = 3;
	}

	CYCLE(3)
		regPC = mTmpWord;
}

// D8: Clear Decimal Flag
void cCpu_Mos_6502::o_Flag_Decimal_Clear() {

	CYCLE(1)
		flagDecimal = false;
}

// 
void cCpu_Mos_6502::o_Compare_Absolute_X() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2) {
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);
		if(flagCarry.get())
			mTmpWord += 0x100;
	}

	CYCLE(3) {
		byte highbyte = mTmpWord >> 8;
		mTmpWord += regX();

		// Page Crossed?
		if( highbyte != (mTmpWord >> 8) )
			++mCycles;
		else
			mCycle = 4;
	}

	CYCLE(4) {
		mTmpByte = mSystem()->busReadByte( mTmpWord );
		mTmpByte -= regA();
		
		if( mTmpByte >= 0 )
			flagCarry = true;
		else
			flagCarry = false;

		registerFlagSet( mTmpByte );

		if(mCycles==4)
			mTmpOpcode = mSystem()->busReadByte( regPC++ );
	}

}
