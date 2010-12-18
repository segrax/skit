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
	void					 o_Jump_Subroutine();					// 20: Jump Subroutine
	void					 o_Return_From_Subroutine();			// 60: Return From Subroutine
	void					 o_Flag_Interrupt_Disable_Set();		// 78: Set Interrupt Disable
	void					 o_Store_Index_X_Absolute();			// 8E: Store X Absolute
	void					 o_Transfer_X_to_StackPtr();			// 9A: Transfer X to Stack
	void					 o_Load_Index_X_Immediate();			// A2: Load X Immediate
	void					 o_Load_A_Absolute_X();					// BD: Load A from Absolute_X
	void					 o_Branch_Not_Equal();					// D0: Branch not Equal
	void					 o_Flag_Decimal_Clear();				// D8: Clear Direction
	void					 o_Compare_Absolute_X();				// DD: Compare

	// Analyse Opcodes
	void					 a_Nop();
	void					 a_Jump_Subroutine();					// 20: Jump Subroutine
	void					 a_Return_From_Subroutine();			// 60: Return From Subroutine
	void					 a_Flag_Interrupt_Disable_Set();		// 78: Set Interrupt Disable
	void					 a_Store_Index_X_Absolute();			// 8E: Store X Absolute
	void					 a_Transfer_X_to_StackPtr();			// 9A: Transfer X to Stack
	void					 a_Load_Index_X_Immediate();			// A2: Analysis Load X Immediate
	void					 a_Load_A_Absolute_X();					// BD: Load A from Absolute_X
	void					 a_Branch_Not_Equal();					// D0: Branch not Equal
	void					 a_Flag_Decimal_Clear();				// D8: Clear Direction
	void					 a_Compare_Absolute_X();				// DD: Compare

};
