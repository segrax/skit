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
	OPCODE(0x08,	o_Push_Flags,					a_Push_Flags,					3);
	OPCODE(0x09,	o_Or_Accumulator,				a_Or_Accumulator,				2);
	OPCODE(0x0D,	o_Or_Accumulator_Absolute,		a_Or_Accumulator_Absolute,		4);
	OPCODE(0x10,	o_Branch_If_Negative_Clear,		a_Branch_If_Negative_Clear,		2);
	OPCODE(0x18,	o_Flag_Carry_Clear,				a_Flag_Carry_Clear,				2);

	OPCODE(0x20,	o_Jump_Subroutine,				a_Jump_Subroutine,				6);
	OPCODE(0x28,	o_Pull_Flags,					a_Pull_Flags,					4);
	OPCODE(0x29,	o_And_Accumulator_Immediate,	a_And_Immediate,				2);
	OPCODE(0x2A,	o_Roll_Accumulator_Left,		a_Roll_Accumulator_Left,		2);

	OPCODE(0x30,	o_Branch_If_Negative_Set,		a_Branch_If_Negative_Set,		2);
	OPCODE(0x38,	o_Flag_Carry_Set,				a_Flag_Carry_Set,				2);

	OPCODE(0x48,	o_Push_Accumulator,				a_Push_Accumulator,				3);
	OPCODE(0x4C,	o_Jump_Absolute,				a_Jump_Absolute,				3);	

	OPCODE(0x58,	o_Flag_Interrupt_Clear,			a_Flag_Interrupt_Clear,			2);

	OPCODE(0x60,	o_Return_From_Subroutine,		a_Return_From_Subroutine,		6);
	OPCODE(0x65,	o_Add_With_Carry_ZeroPage,		a_Add_With_Carry_ZeroPage,		3);
	OPCODE(0x68,	o_Pull_Accumulator,				a_Pull_Accumulator,				4);
	OPCODE(0x69,	o_Add_With_Carry_Immediate,		a_Add_With_Carry_Immediate,		2);
	OPCODE(0x6C,	o_Jump_Indirect,				a_Jump_Indirect,				5);

	OPCODE(0x78,	o_Flag_Interrupt_Disable_Set,	a_Flag_Interrupt_Disable_Set,	2);
	
	OPCODE(0x84,	o_Store_Index_Y_ZeroPage,		a_Store_Index_Y_ZeroPage,		3);
	OPCODE(0x85,	o_Store_Accumulator_ZeroPage,	a_Store_Accumulator_ZeroPage,	3);
	OPCODE(0x86,	o_Store_Index_X_ZeroPage,		a_Store_Index_X_ZeroPage,		3);
	OPCODE(0x88,	o_Decrease_Y,					a_Decrease_Y,					2);
	OPCODE(0x8A,	o_Transfer_Index_X_To_A,		a_Transfer_Index_X_To_A,		2);
	OPCODE(0x8C,	o_Store_Index_Y_Absolute,		a_Store_Index_Y_Absolute,		4);
	OPCODE(0x8D,	o_Store_Accumulator_Absolute,	a_Store_Accumulator_Absolute,	4);
	OPCODE(0x8E,	o_Store_Index_X_Absolute,		a_Store_Index_X_Absolute,		4);
	
	OPCODE(0x90,	o_Branch_If_Carry_Clear,		a_Branch_If_Carry_Clear,		2);
	OPCODE(0x91,	o_Store_Accumulator_Indirect_Y,	a_Store_Accumulator_Indirect_Y,	6);
	OPCODE(0x94,	o_Store_Index_Y_ZeroPage_X,		a_Store_Index_Y_ZeroPage_X,		4);
	OPCODE(0x95,	o_Store_Accumulator_ZeroPage_X, a_Store_Accumulator_ZeroPage_X, 4);
	OPCODE(0x98,	o_Transfer_Index_Y_To_A,		a_Transfer_Index_Y_To_A,		2);
	OPCODE(0x99,	o_Store_Accumulator_Absolute_Y,	a_Store_Accumulator_Absolute_Y, 5);
	OPCODE(0x9A,	o_Transfer_X_to_StackPtr,		a_Transfer_X_to_StackPtr,		2);
	OPCODE(0x9D,	o_Store_Accumulator_Absolute_X,	a_Store_Accumulator_Absolute_X, 5);
	
	OPCODE(0xA0,	o_Load_Index_Y_Immediate,		a_Load_Index_Y_Immediate,		2);
	OPCODE(0xA2,	o_Load_Index_X_Immediate,		a_Load_Index_X_Immediate,		2);
	OPCODE(0xA4,	o_Load_Index_Y_ZeroPage,		a_Load_Index_Y_ZeroPage,		3);
	OPCODE(0xA5,	o_Load_Accumulator_ZeroPage,	a_Load_Accumulator_ZeroPage,	3);
	OPCODE(0xA6,	o_Load_Index_X_ZeroPage,		a_Load_Index_X_ZeroPage,		3);
	OPCODE(0xA8,	o_Transfer_Accumulator_To_Y,	a_Transfer_Accumulator_To_Y,	2);
	OPCODE(0xA9,	o_Load_Accumulator_Immediate,	a_Load_Accumulator_Immediate,	2);
	OPCODE(0xAA,	o_Transfer_Accumulator_To_X,	a_Transfer_Accumulator_To_X,	2);
	OPCODE(0xAC,	o_Load_Index_Y_Absolute,		a_Load_Index_X_Absolute,		4);
	OPCODE(0xAD,	o_Load_Accumulator_Absolute,	a_Load_Accumulator_Absolute,	4);
	OPCODE(0xAE,	o_Load_Index_X_Absolute,		a_Load_Index_X_Absolute,		4);

	OPCODE(0xB0,	o_Branch_If_CarrySet,			a_Branch_If_CarrySet,			2);
	OPCODE(0xB1,	o_Load_Accumulator_Indirect_Y,	a_Load_Accumulator_Indirect_Y,	5);
	OPCODE(0xB4,	o_Load_Index_Y_ZeroPage_X,		a_Load_Index_Y_ZeroPage_X,		4);
	OPCODE(0xB5,	o_Load_Accumulator_ZeroPage_X,	a_Load_Accumulator_ZeroPage_X,	4);
	OPCODE(0xB9,	o_Load_Accumulator_Absolute_Y,	a_Load_Accumulator_Absolute_Y,	4);
	OPCODE(0xBD,	o_Load_A_Absolute_X,			a_Load_A_Absolute_X,			4);

	OPCODE(0xC4,	o_Compare_Index_Y_ZeroPage,		a_Compare_Index_Y_ZeroPage,		3);
	OPCODE(0xC5,	o_Compare_Accumulator_ZeroPage,	a_Compare_Accumulator_ZeroPage,	3);
	OPCODE(0xC6,	o_Decrease_Memory_ZeroPage,		a_Decrease_ZeroPage,			5);
	OPCODE(0xC8,	o_Increase_Y,					a_Increase_Y,					2);
	OPCODE(0xC9,	o_Compare_Accumulator_Immediate,a_Compare_Accumulator_Immediate,2);
	OPCODE(0xCA,	o_Decrease_X,					a_Decrease_X,					2);
	OPCODE(0xCE,	o_Decrease_Memory_Absolute,		a_Decrease_Memory_Absolute,		6);

	OPCODE(0xD0,	o_Branch_Not_Equal,				a_Branch_Not_Equal,				2);
	OPCODE(0xD1,	o_Compare_Indirect_Y,			a_Compare_Indirect_Y,			5);
	OPCODE(0xD8,	o_Flag_Decimal_Clear,			a_Flag_Decimal_Clear,			2);
	OPCODE(0xDD,	o_Compare_Absolute_X,			a_Compare_Absolute_X,			4);

	OPCODE(0xE0,	o_Compare_Index_X_Immediate,	a_Compare_Index_X_Immediate,	2);
	OPCODE(0xE6,	o_Increment_Memory_ZeroPage,	a_Increment_Memory_ZeroPage,	5);
	OPCODE(0xE8,	o_Increase_X,					a_Increase_X,					2);
	OPCODE(0xE9,	o_Subtract_With_Carry_Immediate,a_Subtract_With_Carry_Immediate,2);
	OPCODE(0xEE,	o_Increase_Memory_Absolute,		a_Increase_Memory_Absolute,		6);

	OPCODE(0xF0,	o_Branch_If_Zero_Set,			a_Branch_Equal,					2);
}

void cCpu_Mos_6502::o_Unknown_Opcode() {
	std::stringstream msg;

	msg << debug_CPU_Info_String();
	msg << "\n" << "Unknown Opcode! ";

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

// 08: 
void cCpu_Mos_6502::o_Push_Flags() {
	CYCLE(2)
		stackPush( regFL.value() | 0x10 );

}

// 09: Or
void cCpu_Mos_6502::o_Or_Accumulator() {

	CYCLE(1)
		regA |= mSystem()->busReadByte( regPC++ );

}

// 0D: Or
void cCpu_Mos_6502::o_Or_Accumulator_Absolute() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);
	
	CYCLE(3)
		regA |= mSystem()->busReadByte( mTmpWord );
}

// 10: 
void cCpu_Mos_6502::o_Branch_If_Negative_Clear() {

	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		if( flagNegative == false )
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

// 18: 
void cCpu_Mos_6502::o_Flag_Carry_Clear() {
	CYCLE(1)
		flagCarry = false;
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

// 28: 
void cCpu_Mos_6502::o_Pull_Flags() {
	CYCLE(1);
	CYCLE(2);
	CYCLE(3)
		regFL.valueSet( stackPop() );
}

// 29: 
void cCpu_Mos_6502::o_And_Accumulator_Immediate() {

	CYCLE(1)
		regA &= mSystem()->busReadByte( regPC++ );

}			

// 2A: 
void cCpu_Mos_6502::o_Roll_Accumulator_Left() {

	CYCLE(1) {
		bool cf = flagCarry.get();

		if( regA() & 0x80 )
			flagCarry = true;
		else
			flagCarry = false;

		regA <<= 1;

		if(cf)
			regA |= 0x01;
	}
}

// 30: 
void cCpu_Mos_6502::o_Branch_If_Negative_Set() {
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		if( flagNegative == true )
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

// 38: 
void cCpu_Mos_6502::o_Flag_Carry_Set() {
	CYCLE(1)
		flagCarry = true;
}

// 48: 
void cCpu_Mos_6502::o_Push_Accumulator() {
	CYCLE(2)
		stackPush( regA());

}

// 4C: Jump
void cCpu_Mos_6502::o_Jump_Absolute() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2) {
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);
		regPC = mTmpWord;
	}
}

// 58: 
void cCpu_Mos_6502::o_Flag_Interrupt_Clear() {
	
	CYCLE(1)
		flagInterrupt = false;

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

// 65:
void cCpu_Mos_6502::o_Add_With_Carry_ZeroPage() {
	CYCLE(1) 
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2) {
		mTmpByte = mSystem()->busReadByte( mTmpByte );

		o__AddWithCarry();
	}

}

// 68: 
void cCpu_Mos_6502::o_Pull_Accumulator() {
	CYCLE(1);
	CYCLE(2);
	CYCLE(3)
		regA = stackPop();
}

// 69: 
void cCpu_Mos_6502::o_Add_With_Carry_Immediate() {

	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );

		o__AddWithCarry();
	}

}

// 6C:
void cCpu_Mos_6502::o_Jump_Indirect() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);
	
	CYCLE(3)
		mTmpByte = mSystem()->busReadByte( mTmpWord++ );

	CYCLE(4) 
		regPC = mTmpByte | (mSystem()->busReadByte( mTmpWord ) << 8);

}

// 78: Disable Interrupt Flag
void cCpu_Mos_6502::o_Flag_Interrupt_Disable_Set() {

	CYCLE(1)
		flagInterrupt = true;
}

// 84: 
void cCpu_Mos_6502::o_Store_Index_Y_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mSystem()->busWriteByte( mTmpByte, regY() );
}

// 85: 
void cCpu_Mos_6502::o_Store_Accumulator_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mSystem()->busWriteByte( mTmpByte, regA() );

}

// 86: 
void cCpu_Mos_6502::o_Store_Index_X_ZeroPage() {

	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mSystem()->busWriteByte( mTmpByte, regX() );
}

// 88: 
void cCpu_Mos_6502::o_Decrease_Y() {
	
	CYCLE(1)
		--regY;
}

// 8A: 
void cCpu_Mos_6502::o_Transfer_Index_X_To_A() {

	CYCLE(1)
		regA = regX();
}

// 8C: 
void cCpu_Mos_6502::o_Store_Index_Y_Absolute() {
	
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mSystem()->busWriteByte( mTmpWord, regY() );
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

// 90: 
void cCpu_Mos_6502::o_Branch_If_Carry_Clear() {
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		if( flagCarry == false )
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

// 91:
void cCpu_Mos_6502::o_Store_Accumulator_Indirect_Y() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord = mSystem()->busReadByte( mTmpByte++ );

	CYCLE(3)
		mTmpWord |= (mSystem()->busReadByte( mTmpByte ) << 8);

	CYCLE(4)
		mTmpWord += regY();

	CYCLE(5)
		mSystem()->busWriteByte( mTmpWord, regA() );
}

// 94: 
void cCpu_Mos_6502::o_Store_Index_Y_ZeroPage_X() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpByte += regX();

	CYCLE(3)
		mSystem()->busWriteByte( mTmpByte, regY() );
}

// 95: 
void cCpu_Mos_6502::o_Store_Accumulator_ZeroPage_X() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpByte += regX();

	CYCLE(3)
		mSystem()->busWriteByte( mTmpByte, regA() );
}

// 98:
void cCpu_Mos_6502::o_Transfer_Index_Y_To_A() {

	CYCLE(1)
		regA = regY();
}
 
// 99: 
void cCpu_Mos_6502::o_Store_Accumulator_Absolute_Y() {
	
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mTmpWord += regY();

	CYCLE(4)
		mSystem()->busWriteByte( mTmpWord, regA() );
}

// 9A: Transfer X to StackPtr
void cCpu_Mos_6502::o_Transfer_X_to_StackPtr() {
	
	CYCLE(1)
		regSP = regX();
}

// 9D: 
void cCpu_Mos_6502::o_Store_Accumulator_Absolute_X() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mTmpWord += regX();

	CYCLE(4)
		mSystem()->busWriteByte( mTmpWord, regA() );
}

// A0: 
void cCpu_Mos_6502::o_Load_Index_Y_Immediate() {
	
	CYCLE(1)
		regY = mSystem()->busReadByte( regPC++ );

}

// A2: Load X Register with Immediate Value
void cCpu_Mos_6502::o_Load_Index_X_Immediate() {

	CYCLE(1)
		regX = mSystem()->busReadByte( regPC++ );
}

// A4: 
void cCpu_Mos_6502::o_Load_Index_Y_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		regY = mSystem()->busReadByte( mTmpByte );
}

// A5: 
void cCpu_Mos_6502::o_Load_Accumulator_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		regA = mSystem()->busReadByte( mTmpByte );
}

// A6: 
void cCpu_Mos_6502::o_Load_Index_X_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		regX = mSystem()->busReadByte( mTmpByte );
}

// A8:
void cCpu_Mos_6502::o_Transfer_Accumulator_To_Y() {

	CYCLE(1)
		regY = regA();
}

// A9: Load Accumulator with Immediate Value
void cCpu_Mos_6502::o_Load_Accumulator_Immediate() {

	CYCLE(1)
		regA = mSystem()->busReadByte( regPC++ );
}

// AA:
void cCpu_Mos_6502::o_Transfer_Accumulator_To_X() {

	CYCLE(1)
		regX = regA();
}

// AC: 
void cCpu_Mos_6502::o_Load_Index_Y_Absolute() {

	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		regY = mSystem()->busReadByte( mTmpWord );
	
}

// AD: 
void cCpu_Mos_6502::o_Load_Accumulator_Absolute() {

	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		regA = mSystem()->busReadByte( mTmpWord );
	
}

// AE:
void cCpu_Mos_6502::o_Load_Index_X_Absolute() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		regX = mSystem()->busReadByte( mTmpWord );
}

// B0: 
void cCpu_Mos_6502::o_Branch_If_CarrySet() {
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		if( flagCarry == true )
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

// B1: 
void cCpu_Mos_6502::o_Load_Accumulator_Indirect_Y() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord = mSystem()->busReadByte( mTmpByte++ );

	CYCLE(3)
		mTmpWord |= (mSystem()->busReadByte( mTmpByte ) << 8);

	CYCLE(4) {

		byte highbyte = mTmpWord >> 8;
		mTmpWord += regY();

		// Page Crossed?
		if( highbyte != (mTmpWord >> 8) )
			++mCycles;
		else
			mCycle = 5;
	}

	CYCLE(5)
		regA = mSystem()->busReadByte( mTmpWord );
}

// B4: 
void cCpu_Mos_6502::o_Load_Index_Y_ZeroPage_X() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpByte += regX();

	CYCLE(3)
		regY = mSystem()->busReadByte( mTmpByte );
}

// B5: 
void cCpu_Mos_6502::o_Load_Accumulator_ZeroPage_X() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpByte += regX();

	CYCLE(3)
		regA = mSystem()->busReadByte( mTmpByte );
}

// B9: 
void cCpu_Mos_6502::o_Load_Accumulator_Absolute_Y() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3) {
		byte highbyte = mTmpWord >> 8;
		mTmpWord += regY();

		// Page Crossed?
		if( highbyte != (mTmpWord >> 8) )
			++mCycles;
		else {
			mCycle = 4;
		}
	}

	CYCLE(4)
		regA = mSystem()->busReadByte( mTmpWord );
}

// BD: Load A from Absolute_X
void cCpu_Mos_6502::o_Load_A_Absolute_X() {

	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

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

// C4:
void cCpu_Mos_6502::o_Compare_Index_Y_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2) {
		mTmpByte =  mSystem()->busReadByte( mTmpByte );
		if( mTmpByte >= regY() )
			flagCarry = true;
		else
			flagCarry = false;

		mTmpByte -= regY();
		registerFlagSet( mTmpByte );
	}
}

// C5:
void cCpu_Mos_6502::o_Compare_Accumulator_ZeroPage() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2) {
		mTmpByte =  mSystem()->busReadByte( mTmpByte );

		if( mTmpByte >= regA() )
			flagCarry = true;
		else
			flagCarry = false;

		mTmpByte -= regA();
		registerFlagSet( mTmpByte );
	}
}

// C6:
void cCpu_Mos_6502::o_Decrease_Memory_ZeroPage() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpByte = mSystem()->busReadByte( mTmpWord );

	CYCLE(3)
		--mTmpByte;

	CYCLE(4)
		mSystem()->busWriteByte( mTmpWord, mTmpByte );
}

// C8: 
void cCpu_Mos_6502::o_Increase_Y() {

	CYCLE(1)
		++regY;
}

// C9:
void cCpu_Mos_6502::o_Compare_Accumulator_Immediate() {
	
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		
		if( mTmpByte >= regA() )
			flagCarry = true;
		else
			flagCarry = false;

		mTmpByte -= regA();
		registerFlagSet( mTmpByte );
	}

}

// CA: 
void cCpu_Mos_6502::o_Decrease_X() {

	CYCLE(1)
		--regX;

}

// CE:
void cCpu_Mos_6502::o_Decrease_Memory_Absolute() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mTmpByte = mSystem()->busReadByte( mTmpWord );

	CYCLE(4)
		--mTmpByte;

	CYCLE(5)
		mSystem()->busWriteByte( mTmpWord, mTmpByte );
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

// D1: 
void cCpu_Mos_6502::o_Compare_Indirect_Y() {
	CYCLE(1)
		mTmpByte = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord = mSystem()->busReadByte( mTmpByte++ );

	CYCLE(3)
		mTmpWord |= (mSystem()->busReadByte( mTmpByte ) << 8);

	CYCLE(4) {
		byte highbyte = mTmpWord >> 8;
		mTmpWord += (char) regY();

		// Page Crossed?
		if( highbyte != (mTmpWord >> 8) )
			++mCycles;
		else
			mCycle = 5;
	}

	CYCLE(5) {
		mTmpByte = mSystem()->busReadByte( mTmpWord );

		if( mTmpByte >= regA() )
			flagCarry = true;
		else
			flagCarry = false;
		
		mTmpByte -= regA();
		registerFlagSet( mTmpByte );
	}
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

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

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
		
		if( mTmpByte >= regA() )
			flagCarry = true;
		else
			flagCarry = false;

		mTmpByte -= regA();
		registerFlagSet( mTmpByte );

		if(mCycles==4)
			mTmpOpcode = mSystem()->busReadByte( regPC++ );
	}

}

// E0:
void cCpu_Mos_6502::o_Compare_Index_X_Immediate() {
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );

		if( mTmpByte >= regX() )
			flagCarry = true;
		else
			flagCarry = false;

		mTmpByte -= regX();
		registerFlagSet( mTmpByte );
	}
}

// E6: 
void cCpu_Mos_6502::o_Increment_Memory_ZeroPage() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpByte = mSystem()->busReadByte( mTmpWord );

	CYCLE(3)
		++mTmpByte;

	CYCLE(4)
		mSystem()->busWriteByte( mTmpWord, mTmpByte );
}

// E8: 
void cCpu_Mos_6502::o_Increase_X(){						
	CYCLE(1)
		++regX;
}

// E9:
void cCpu_Mos_6502::o_Subtract_With_Carry_Immediate() {
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );

		o__SubWithCarry();
	}
}

// EE:
void cCpu_Mos_6502::o_Increase_Memory_Absolute() {
	CYCLE(1)
		mTmpWord = mSystem()->busReadByte( regPC++ );

	CYCLE(2)
		mTmpWord |= (mSystem()->busReadByte( regPC++ ) << 8);

	CYCLE(3)
		mTmpByte = mSystem()->busReadByte( mTmpWord );

	CYCLE(4)
		++mTmpByte;

	CYCLE(5)
		mSystem()->busWriteByte( mTmpWord, mTmpByte );
}

// F0: 
void cCpu_Mos_6502::o_Branch_If_Zero_Set() {
	CYCLE(1) {
		mTmpByte = mSystem()->busReadByte( regPC++ );
		if( flagZero == true )
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
