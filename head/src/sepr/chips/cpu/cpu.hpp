class cChip_Opcode;

class cCpu : public cDevice {
private:

protected:
	size_t				 mOpcodeCount, mOpcodeNumber;
	cChip_Opcode		*mOpcodes;
	cChip_Opcode		*mOpcodeCurrent;
	cChip_Opcode		*mOpcode_Unknown,		*mOpcode_Reset;

	cChip_Registers		 mRegisters;

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

	virtual		void	 registerFlagSet( size_t pData ) = 0;

	const cChip_Opcode	*mOpcodeCurrentGet() { return mOpcodeCurrent; }

protected:

	virtual void		 o_Reset();
	virtual void		 o_Unknown_Opcode();
};
