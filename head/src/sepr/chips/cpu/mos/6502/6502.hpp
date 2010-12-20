#define		regA	(*mRegA)
#define		regX	(*mRegX)
#define		regY	(*mRegY)

#define		regFL	(*mRegFL)
#define		regSP	(*mRegStack)	
#define		regPC	(*mRegPC)

#define		flagInterrupt	(*mFlagInterrupt)
#define		flagDecimal		(*mFlagDecimal)
#define		flagCarry		(*mFlagCarry)
#define		flagZero		(*mFlagZero)
#define		flagNegative	(*mFlagNegative)
#define		flagOverflow	(*mFlagOverflow)
#define		flagReserved	(*mFlagReserved)

enum eChip_Cpu_Mos_6502_Flags {
	flagCARRY		= 0x01,
	flagZERO		= 0x02,
	flagIRQ_DISABLE = 0x04,
	flagDECIMAL		= 0x08,
	flagBREAK		= 0x10,
	flagRESERVED	= 0x20,
	flagOVERFLOW	= 0x40,
	flagNEGATIVE	= 0x80,
};

class cCpu_Mos_6502 : public cCpu {
private:
	cChip_Register_Byte		*mRegA, *mRegX, *mRegY, *mRegStack;
	cChip_Register_Word		*mRegPC;
	cChip_Register_Flags	*mRegFL;

	cChip_Register_Flag		*mFlagNegative, *mFlagOverflow, *mFlagReserved, *mFlagBreak;
	cChip_Register_Flag		*mFlagDecimal,	*mFlagInterrupt,*mFlagZero,		*mFlagCarry;


	byte					 mTmpOpcode,	mTmpByte;
	word					 mTmpWord;
	
protected:
	void					 stackPush( byte pData );		// Push a byte onto the stack
	byte					 stackPop();

public:
							 cCpu_Mos_6502( std::string pName, cSepr *pSepr, cDevice *pParent );
							~cCpu_Mos_6502();

	virtual		void		 registerFlagSet( size_t pData );

	void					 cycle();
	void					 reset();								// Set opcode to reset opcode

protected:
	virtual std::string		 debug_CPU_Info_String();

	virtual void			 opcodesPrepare();
	virtual void			 registersPrepare();

	virtual void			 o_Reset();								// Reset Routine
	virtual void			 o_Unknown_Opcode();					// Unknown Opcode

	// Opcodes
	void					 o_Nop();
	void					 o__AddWithCarry();
	void					 o__SubWithCarry();

	void					 o_Push_Flags();						// 08: 
	void					 o_Or_Accumulator();					// 09: Or
	void					 o_Or_Accumulator_Absolute();			// 0D: Or
	void					 o_Branch_If_Negative_Clear();			// 10: 
	void					 o_Arith_Shift_Left_ZeroPage_X();		// 16:
	void					 o_Flag_Carry_Clear();					// 18: 
	void					 o_Jump_Subroutine();					// 20: Jump Subroutine
	void					 o_Pull_Flags();						// 28: 
	void					 o_And_Accumulator_Immediate();			// 29: 
	void					 o_Roll_Accumulator_Left();				// 2A: 
	void					 o_Branch_If_Negative_Set();			// 30:
	void					 o_Flag_Carry_Set();					// 38: 
	void					 o_Logical_Shift_Right_ZeroPage();		// 46: 
	void					 o_Push_Accumulator();					// 48:
	void					 o_Exclusive_Or();						// 49: 
	void					 o_Jump_Absolute();						// 4C: Jump
	void					 o_Logical_Shift_Right_ZeroPage_X();	// 56: 
	void					 o_Flag_Interrupt_Clear();				// 58: 
	void					 o_Return_From_Subroutine();			// 60: Return From Subroutine
	void					 o_Add_With_Carry_ZeroPage();			// 65:
	void					 o_Pull_Accumulator();					// 68: 
	void					 o_Add_With_Carry_Immediate();			// 69: 
	void					 o_Jump_Indirect();						// 6C: 
	void					 o_Flag_Interrupt_Disable_Set();		// 78: Set Interrupt Disable
	void					 o_Store_Index_Y_ZeroPage();			// 84:
	void					 o_Store_Accumulator_ZeroPage();		// 85: 
	void					 o_Store_Index_X_ZeroPage();			// 86: 
	void					 o_Decrease_Y();						// 88: 
	void					 o_Transfer_Index_X_To_A();				// 8A: 
	void					 o_Store_Index_Y_Absolute();			// 8C:
	void					 o_Store_Accumulator_Absolute();		// 8D: 
	void					 o_Store_Index_X_Absolute();			// 8E: Store X Absolute
	void					 o_Branch_If_Carry_Clear();				// 90: 
	void					 o_Store_Accumulator_Indirect_Y();		// 91:
	void					 o_Store_Index_Y_ZeroPage_X();			// 94:
	void					 o_Store_Accumulator_ZeroPage_X();		// 95: 
	void					 o_Transfer_Index_Y_To_A();				// 98: 
	void					 o_Store_Accumulator_Absolute_Y();		// 99: 
	void					 o_Transfer_X_to_StackPtr();			// 9A: Transfer X to Stack
	void					 o_Store_Accumulator_Absolute_X();		// 9D: 
	void					 o_Load_Index_Y_Immediate();			// A0: Load Y Immediate
	void					 o_Load_Index_X_Immediate();			// A2: Load X Immediate
	void					 o_Load_Index_Y_ZeroPage();				// A4: 
	void					 o_Load_Accumulator_ZeroPage();			// A5: 
	void					 o_Load_Index_X_ZeroPage();				// A6: 
	void					 o_Transfer_Accumulator_To_Y();			// A8: 
	void					 o_Load_Accumulator_Immediate();		// A9:
	void					 o_Transfer_Accumulator_To_X();			// AA:
	void					 o_Load_Index_Y_Absolute();				// AC:
	void					 o_Load_Accumulator_Absolute();			// AD:
	void					 o_Load_Index_X_Absolute();				// AE:
	void					 o_Branch_If_CarrySet();				// B0: 
	void					 o_Load_Accumulator_Indirect_Y();		// B1: 
	void					 o_Load_Index_Y_ZeroPage_X();			// B4: 
	void					 o_Load_Accumulator_ZeroPage_X();		// B5: 
	void					 o_Load_Accumulator_Absolute_Y();		// B9: 
	void					 o_Load_A_Absolute_X();					// BD: Load A from Absolute_X
	void					 o_Compare_Index_Y_ZeroPage();			// C4:
	void					 o_Compare_Accumulator_ZeroPage();		// C5:
	void					 o_Decrease_Memory_ZeroPage();			// C6:
	void					 o_Increase_Y();						// C8:
	void					 o_Compare_Accumulator_Immediate();		// C9: 
	void					 o_Decrease_X();						// CA: Decrease X
	void					 o_Decrease_Memory_Absolute();			// CE: 
	void					 o_Branch_Not_Equal();					// D0: Branch not Equal
	void					 o_Compare_Indirect_Y();				// D1: 
	void					 o_Flag_Decimal_Clear();				// D8: Clear Direction
	void					 o_Compare_Absolute_X();				// DD: Compare
	void					 o_Compare_Index_X_Immediate();			// E0: 
	void					 o_Subtract_With_Carry_ZeroPage();		// E5:
	void					 o_Increment_Memory_ZeroPage();			// E6: 
	void					 o_Increase_X();						// E8: 
	void					 o_Subtract_With_Carry_Immediate();		// E9:
	void					 o_Increase_Memory_Absolute();			// EE: 
	void					 o_Branch_If_Zero_Set();						// F0: Branch If Equal

	// Analyse Opcodes
	void					 a_Nop();
	void					 a_Push_Flags();						// 08: 
	void					 a_Or_Accumulator();					// 09: Or
	void					 a_Or_Accumulator_Absolute();			// 0D: Or				 
	void					 a_Branch_If_Negative_Clear();			// 10: 
	void					 a_Arith_Shift_Left_ZeroPage_X();		// 16:
	void					 a_Flag_Carry_Clear();					// 18: 
	void					 a_Jump_Subroutine();					// 20: Jump Subroutine
	void					 a_Pull_Flags();						// 28: 
	void					 a_And_Immediate();						// 29: 
	void					 a_Roll_Accumulator_Left();				// 2A: 
	void					 a_Branch_If_Negative_Set();			// 30:
	void					 a_Flag_Carry_Set();					// 38: 
	void					 a_Logical_Shift_Right_ZeroPage();		// 46: 
	void					 a_Push_Accumulator();					// 48:
	void					 a_Exclusive_Or();						// 49: 
	void					 a_Jump_Absolute();						// 4C: Jump
	void					 a_Logical_Shift_Right_ZeroPage_X();	// 56: 
	void					 a_Flag_Interrupt_Clear();				// 58: 
	void					 a_Return_From_Subroutine();			// 60: Return From Subroutine
	void					 a_Add_With_Carry_ZeroPage();			// 65:
	void					 a_Pull_Accumulator();					// 68: 
	void					 a_Add_With_Carry_Immediate();			// 69: 
	void					 a_Jump_Indirect();						// 6C:
	void					 a_Flag_Interrupt_Disable_Set();		// 78: Set Interrupt Disable
	void					 a_Store_Index_Y_ZeroPage();			// 84:
	void					 a_Store_Accumulator_ZeroPage();		// 85: 
	void					 a_Store_Index_X_ZeroPage();			// 86: 
	void					 a_Decrease_Y();						// 88: 
	void					 a_Transfer_Index_X_To_A();				// 8A: 
	void					 a_Store_Index_Y_Absolute();			// 8C:
	void					 a_Store_Accumulator_Absolute();		// 8D: 
	void					 a_Store_Index_X_Absolute();			// 8E: Store X Absolute
	void					 a_Branch_If_Carry_Clear();				// 90: 
	void					 a_Store_Accumulator_Indirect_Y();		// 91:
	void					 a_Store_Index_Y_ZeroPage_X();			// 94:
	void					 a_Store_Accumulator_ZeroPage_X();		// 95: 
	void					 a_Transfer_Index_Y_To_A();				// 98: 
	void					 a_Store_Accumulator_Absolute_Y();		// 99: 
	void					 a_Transfer_X_to_StackPtr();			// 9A: Transfer X to Stack
	void					 a_Store_Accumulator_Absolute_X();		// 9D: 
	void					 a_Load_Index_Y_Immediate();			// A0: Load Y Immediate
	void					 a_Load_Index_X_Immediate();			// A2: Load X Immediate
	void					 a_Load_Index_Y_ZeroPage();				// A4: 
	void					 a_Load_Accumulator_ZeroPage();			// A5: 
	void					 a_Load_Index_X_ZeroPage();				// A6:
	void					 a_Transfer_Accumulator_To_Y();			// A8: 
	void					 a_Load_Accumulator_Immediate();		// A9: 
	void					 a_Transfer_Accumulator_To_X();			// AA:
	void					 a_Load_Index_Y_Absolute();				// AC:
	void					 a_Load_Accumulator_Absolute();			// AD: 
	void					 a_Load_Index_X_Absolute();				// AE:
	void					 a_Branch_If_CarrySet();				// B0: 
	void					 a_Load_Accumulator_Indirect_Y();		// B1: 
	void					 a_Load_Index_Y_ZeroPage_X();			// B4: 
	void					 a_Load_Accumulator_ZeroPage_X();		// B5: 
	void					 a_Load_Accumulator_Absolute_Y();		// B9: 
	void					 a_Load_A_Absolute_X();					// BD: Load A from Absolute_X
	void					 a_Compare_Index_Y_ZeroPage();			// C4:
	void					 a_Compare_Accumulator_ZeroPage();		// C5:
	void					 a_Decrease_ZeroPage();					// C6:
	void					 a_Increase_Y();						// C8: 
	void					 a_Compare_Accumulator_Immediate();		// C9: 
	void					 a_Decrease_X();						// CA: Decrease X
	void					 a_Decrease_Memory_Absolute();			// CE: 
	void					 a_Branch_Not_Equal();					// D0: Branch not Equal
	void					 a_Compare_Indirect_Y();				// D1: 
	void					 a_Flag_Decimal_Clear();				// D8: Clear Direction
	void					 a_Compare_Absolute_X();				// DD: Compare
	void					 a_Compare_Index_X_Immediate();			// E0: 
	void					 a_Subtract_With_Carry_ZeroPage();		// E5:
	void					 a_Increment_Memory_ZeroPage();			// E6: 
	void					 a_Increase_X();						// E8: 
	void					 a_Subtract_With_Carry_Immediate();		// E9:
	void					 a_Increase_Memory_Absolute();			// EE: 
	void					 a_Branch_Equal();						// F0: Branch If Equal
};
