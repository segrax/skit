
class cCia_Mos_6526 : public cDevice {
private:
	bool				 mHalted;
	byte 				 mRegPeripheralDataA, mRegPeripheralDataB;
	byte 				 mRegDataDirectionA, mRegDataDirectionB;

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
						 cCia_Mos_6526( std::string pName, cSepr *pSepr, cDevice *pParent   ); 
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );

	void				 interruptFire( size_t pInterrupt );
	void				 TimeOfDayCount();
	
	size_t				 cycle();
	void				 reset();

};
