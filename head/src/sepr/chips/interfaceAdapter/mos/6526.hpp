
class cCia_Mos_6526 : public cDevice {
protected:

	bool				 mHalted;
	byte 				 mRegPeripheralDataA, mRegPeripheralDataB;
	byte 				 mRegDataDirectionA, mRegDataDirectionB;

	cPort<byte>			*mPortA, *mPortB;

	word 				 mRegTimerA, mRegTimerLatchA, mRegTimerB, mRegTimerLatchB;
	dword				 mRegTimerTmp;

	byte 				 mRegTODSeconds10th, mRegTODSeconds, mRegTODMinutes, mRegTODHours, mRegTODDivider;
	byte 				 mRegAlarmSeconds10th, mRegAlarmSeconds, mRegAlarmMinutes, mRegAlarmHours;

	byte 				 mRegSerialData;
	byte				 mRegInterruptControl, mRegInterruptControlMask;
	byte				 mRegControlA, mRegControlB;

	bool				 mRegTimerA_phiCount,	mRegTimerB_phiCount,	mRegTimerB_underflowCount;

protected:

public:
						 cCia_Mos_6526( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent   ); 
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );

	void				 setDataPortA( cPort<byte> *pPort ) { mPortA = pPort; }
	void				 setDataPortB( cPort<byte> *pPort ) { mPortB = pPort; }

	void				 interruptFire( size_t pInterrupt );
	void				 TimeOfDayCount();
	
	size_t				 cycle();
	void				 reset();

};
