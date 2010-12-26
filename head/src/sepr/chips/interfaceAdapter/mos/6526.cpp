#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "6526.hpp"
#include "systems/system.hpp"

cCia_Mos_6526::cCia_Mos_6526( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent  ) : cDevice( pName, pSepr, pSystem, pParent ) {

}
		
byte cCia_Mos_6526::busReadByte( dword		pAddress ) {
	pAddress &= 0x0F;

	switch(pAddress) {
		case	0x00:					// 
			return mRegPeripheralDataA;

		case	0x01:					//
			return mRegPeripheralDataB;

		case	0x02:
			return mRegDataDirectionA;

		case	0x03:
			return mRegDataDirectionB;
			
		case	0x04:
			return mRegTimerA & 0xFF;
			
		case	0x05:
			return mRegTimerA >> 8;

		case	0x06:
			return mRegTimerB & 0xFF;

		case	0x07:
			return mRegTimerB >> 8;

		case	0x08:
			mHalted = false;
			return mRegTODSeconds10th;

		case	0x09:
			return mRegTODSeconds;

		case	0x0A:
			return mRegTODMinutes;

		case	0x0B:
			mHalted = true;
			return mRegTODHours;

		case	0x0C:
			return mRegSerialData;

		case	0x0D: {
						byte ret = mRegInterruptControl;
						mRegInterruptControl = 0;
						
						//mSystem->interruptAdd( mNameGet(), this );

						return ret;
						}
		case	0x0E:
			return mRegControlA;

		case	0x0F:
			return mRegControlB;
	}

	return 0;
}

void cCia_Mos_6526::busWriteByte( dword pAddress, byte  pData ) {
	pAddress &= 0x0F;

	switch(pAddress) {
		case	0x00:					// 
			mRegPeripheralDataA = pData;
			break;
		case	0x01:					//
			mRegPeripheralDataB = pData;
			break;
		case	0x02:
			mRegDataDirectionA = pData;
			break;
		case	0x03:
			mRegDataDirectionB = pData;
			break;
		case	0x04:
			mRegTimerLatchA = (mRegTimerLatchA & 0xFF00) | pData;
			break;

		case	0x05:
			mRegTimerLatchA = (mRegTimerLatchA & 0xFF) | (pData << 8);
			if (!(mRegControlA & 1))	// Reload timer if stopped
				mRegTimerA = mRegTimerLatchA;
			break;

		case	0x06:
			mRegTimerLatchB = (mRegTimerLatchB & 0xFF00) | pData;
			break;

		case	0x07:
			mRegTimerLatchB = (mRegTimerLatchB & 0xFF) | (pData << 8);
			if (!(mRegControlB & 1))	// Reload timer if stopped
				mRegTimerB = mRegTimerLatchB;
			break;

		case	0x08:	// Alarm / Time of day
			if( mRegControlB & 0x80 )
				mRegAlarmSeconds10th = pData & 0x0F;	
			else
				mRegTODSeconds10th = pData & 0x0F;
			break;

		case	0x09:
			if( mRegControlB & 0x80 )
				mRegAlarmSeconds = pData & 0x7F;
			else
				mRegTODSeconds = pData & 0x7F;
			break;

		case	0x0A:
			if( mRegControlB & 0x80 )
				mRegAlarmMinutes = pData & 0x7F;
			else
				mRegTODMinutes = pData & 0x7F;
			break;

		case	0x0B:
			if( mRegControlB & 0x80 )
				mRegTODHours = pData & 0x9F;
			else
				mRegAlarmHours = pData & 0x9F;
			break;

		case	0x0C:
			mRegSerialData = pData;
			interruptFire(8);
			break;

		case	0x0D:

			if(pData & 0x80) {
				// Turn on any bits which are enabled in pData
				mRegInterruptControlMask |= pData & 0x7F;

				if( mRegInterruptControl & (mRegInterruptControlMask & 0x1F) ) {
					mRegInterruptControl |= 0x80;
					
					mSystem->interruptAdd( mNameGet(), this );
				}

			} else
				// Turn off the bits which are enabled in pData
				mRegInterruptControlMask &= ~pData;

			break;

		case	0x0E:
			mRegControlA = pData & 0xEF;
			if( pData & 0x10 )
				mRegTimerA = mRegTimerLatchA;

			mRegTimerA_phiCount =( (pData & 0x21) == 0x01);
			break;

		case	0x0F:
			mRegControlB = pData & 0xEF;
			if( pData & 0x10 )
				mRegTimerB = mRegTimerLatchB;
				
			mRegTimerB_phiCount		= ((pData & 0x61) == 0x01);
			mRegTimerB_underflowCount = ((pData & 0x61) == 0x41);
			break;
	}
}

size_t cCia_Mos_6526::cycle() {

	// Timer A
	if (mRegTimerA_phiCount) {
		mRegTimerA = mRegTimerTmp = mRegTimerA - 1;		// Decrement timer

		if (mRegTimerTmp > 0xffff) {			// Underflow?
			mRegTimerA = mRegTimerLatchA;			// Reload timer

			if (mRegControlA & 8) {			// One-shot?
				mRegControlA &= 0xfe;
				mRegTimerA_phiCount = false;
			}
			interruptFire(1);
			if (mRegTimerB_underflowCount) {		// Timer B counting underflows of Timer A?

				mRegTimerB = mRegTimerTmp = mRegTimerB - 1;	// tmp = --tb doesn't work
				if (mRegTimerTmp > 0xffff) 
					goto tb_underflow;
			}
		}
	}

	// Timer B
	if (mRegTimerB_phiCount) {
		mRegTimerB = mRegTimerTmp = mRegTimerB - 1;		// Decrement timer

		if (mRegTimerTmp > 0xffff) {			// Underflow?
	tb_underflow:
			mRegTimerB = mRegTimerLatchB;

			if (mRegControlB & 8) {			// One-shot?
				mRegControlB &= 0xfe;
				mRegTimerB_phiCount = false;
				mRegTimerB_underflowCount = false;
			}
			interruptFire(2);
		}
	}

	return 0;
}

void cCia_Mos_6526::TimeOfDayCount() {
	byte lo, hi;

	// Decrement frequency divider
	if (mRegTODDivider)
		mRegTODDivider--;

	else {
		// Reload divider according to 50/60 Hz flag
		if (mRegControlA & 0x80)
			mRegTODDivider = 4;
		else
			mRegTODDivider = 5;

		mRegTODSeconds10th++;
		if (mRegTODSeconds10th > 9) {
			mRegTODSeconds10th = 0;

			// Seconds
			lo = (mRegTODSeconds & 0x0f) + 1;
			hi = mRegTODSeconds >> 4;
			if (lo > 9) {
				lo = 0;
				hi++;
			}
			if (hi > 5) {
				mRegTODSeconds = 0;

				// Minutes
				lo = (mRegTODMinutes & 0x0f) + 1;
				hi = mRegTODMinutes >> 4;
				if (lo > 9) {
					lo = 0;
					hi++;
				}
				if (hi > 5) {
					mRegTODMinutes = 0;

					// Hours
					lo = (mRegTODHours & 0x0f) + 1;
					hi = (mRegTODHours >> 4) & 1;
					mRegTODHours &= 0x80;		// Keep AM/PM flag
					if (lo > 9) {
						lo = 0;
						hi++;
					}
					mRegTODHours |= (hi << 4) | lo;
					if ((mRegTODHours & 0x1f) > 0x11)
						mRegTODHours = mRegTODHours & 0x80 ^ 0x80;
				} else
					mRegTODMinutes = (hi << 4) | lo;
			} else
				mRegTODSeconds = (hi << 4) | lo;
		}

		// Alarm?
		if (mRegTODSeconds10th == mRegAlarmSeconds10th && 
			mRegTODSeconds == mRegAlarmSeconds &&
			mRegTODMinutes == mRegAlarmMinutes && 
				mRegTODHours == mRegAlarmHours)

			interruptFire(4);
	}
}

void cCia_Mos_6526::interruptFire( size_t pInterrupt ) {
	mRegInterruptControl |= pInterrupt;

	if( mRegInterruptControl & mRegInterruptControlMask ) {
		mSystem->interruptAdd( mNameGet(), this );
	}

}

void cCia_Mos_6526::reset() {

	mRegPeripheralDataA = mRegPeripheralDataB	= 0xFF;
	mRegDataDirectionA  = mRegDataDirectionB	= 0;

	mRegTimerA = mRegTimerB = mRegTimerTmp = 0xffff;
	mRegTimerLatchA = mRegTimerLatchB = 0;

	mRegTODSeconds10th	= mRegTODSeconds			= mRegTODMinutes				= mRegTODHours	= mRegTODDivider = 0;
	mRegAlarmSeconds10th	= mRegAlarmSeconds		= mRegAlarmMinutes			= mRegAlarmHours	= 0;
	mRegSerialData	 = mRegInterruptControl	= mRegInterruptControlMask	= 0;
	mRegControlA		 = mRegControlB = 0;

		mRegTimerA_phiCount = mRegTimerB_phiCount = mRegTimerB_underflowCount = false;
}
