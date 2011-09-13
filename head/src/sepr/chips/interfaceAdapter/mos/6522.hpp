
class cVia_Mos_6522 : public cDevice {
protected:
	cPort<byte>		    *mPortA, *mPortB;

    byte                 mDataA, mDataB;
	byte			     mDataDirectionPortA, mDataDirectionPortB;
	word 			     mTimerA, mTimerLatchA, mTimerB, mTimerLatchB;
	byte			     mInterruptFlags, mInterruptEnabled;

public:

						 cVia_Mos_6522( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pPart );
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );

	size_t				 cycle();

	void				 timer1FlagClear() { mInterruptFlags  &= ~0x40; }
	void				 timer2FlagClear() { mInterruptFlags  &= ~0x20; }

	void				 timer1FlagSet()	{ mInterruptFlags |= 0x40; }
	void				 timer2FlagSet()	{ mInterruptFlags |= 0x20; }
	
	bool                 timer1InterruptEnabled() { return (mInterruptEnabled & 0x40) != 0; }
    bool                 timer2InterruptEnabled() { return (mInterruptEnabled & 0x20) != 0; } 
};
