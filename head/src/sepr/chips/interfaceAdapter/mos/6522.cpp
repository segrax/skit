#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "io/port/port.hpp"
#include "6522.hpp"
#include "systems/system.hpp"

cVia_Mos_6522::cVia_Mos_6522( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cDevice( pName, pSepr, pSystem, pParent ) {

	mTimerA = mTimerB = 0;
	mInterruptEnabled = 0;
	mPortA = mPortB = 0;
    mDataA = mDataB = 0;
}

size_t cVia_Mos_6522::cycle() {
        if (mTimerA != 0) {
                if (mTimerA <= 1) {
                        // Timer 1 time out
                        timer1FlagSet();
                        if (timer1InterruptEnabled()) {
								mSystem->interruptAdd("VIA_Timer1", this );
                        }
                        // Reload timer
                        mTimerA = 0;
                } else {
                        mTimerA = mTimerA - 1;
                }
        }
        
        if (mTimerB != 0) {
                if (mTimerB <= 1) {
                        // Timer 2 time out
                        timer2FlagSet();
                        if (timer2InterruptEnabled()) {
                                mSystem->interruptAdd("VIA_Timer2", this );
                        }
                        // Reload timer
                        mTimerB = 0;
                } else {
                        mTimerB = mTimerB - 1;
                }
        }

	return 1;
}

byte cVia_Mos_6522::busReadByte( size_t pAddress ) {

	switch(pAddress & 0x0F) {
		case 0x02:
			return mDataDirectionPortB;

		case 0x03:
			return mDataDirectionPortA;
			
		case	0x04:
			timer1FlagClear();
			mSystem->interruptRemove( "VIA_Timer1" );
			return mTimerA & 0xFF;
			
		case	0x05:
			return mTimerA >> 8;

		case	0x06:
			return mTimerLatchA & 0xFF;

		case	0x07:
			return mTimerLatchA >> 8;

		case 0x08:
			timer2FlagClear();
			mSystem->interruptRemove( "VIA_Timer2" );
			return mTimerB & 0xFF;

		case 0x09:
			return mTimerB >> 8;

		case 0x0A:// Shift register
		case 0x0C:// Unused
		case 0x0F:
			return 0;

		case 0x0B:// Auxiliary control register
			return 0;

		case 0x0D:
			return mInterruptFlags | (mInterruptFlags & mInterruptEnabled ? 0x80 : 0);

		case 0x0E:	
			return mInterruptEnabled | 0x80;
	}

	throw new std::exception("Shouldnt reach here");
	return 0;
}

void cVia_Mos_6522::busWriteByte( size_t pAddress, byte pData ) {
	
	switch(pAddress & 0x0F) {
		case	0x02:
			mDataDirectionPortB = pData;
			return;

		case	0x03:
			mDataDirectionPortA = pData;
			return;

		case	0x04:
			mTimerLatchA = (mTimerLatchA & 0xFF00) | pData;
			return;

		case	0x05:
			mTimerLatchA = (mTimerLatchA & 0xFF) | (pData << 8);
			mTimerA = mTimerLatchA;

			timer1FlagClear();
			mSystem->interruptRemove( "VIA_Timer1" );
			return;

		case	0x06:
			mTimerLatchA = (mTimerLatchB & 0xFF00) | pData;
			return;

		case	0x07:
			mTimerLatchA = (mTimerLatchB & 0xFF) | (pData << 8);
			return;

		case	0x08:
			mTimerLatchB = (mTimerLatchB & 0xFF00) | pData;
			return;
			
		case	0x09:
			mTimerLatchB = (mTimerLatchB & 0xFF) | (pData << 8);
			timer2FlagClear();
			mSystem->interruptRemove( "VIA_Timer2" );
			return;

		case	0x0A:
		case	0x0C:
		case	0x0F:
			return;

		case	0x0B:
			return;

		case	0x0D:

			// Turn off the bits which are enabled in pData
			mInterruptFlags &= ~pData;

			return;

		case	0x0E:
			if (pData & 0x80) {
				mInterruptEnabled |= pData & 0x7f;

			} else {
				mInterruptEnabled &= ~pData;
            }

			return;
	}
}
