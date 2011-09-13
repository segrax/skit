// Drive control
class cVia2 : public cVia_Mos_6522 {
	byte mAuxControlReg;

public:
	cVia2(cSepr *pSepr, cSystem *pSystem, cDevice *pDevice ) : cVia_Mos_6522( "VIA2", pSepr, pSystem, pDevice ) {

        mPortB = new cPort<byte>("VIA2_PortB", pSepr, pSystem, pDevice );
	}

	virtual byte		 busReadByte( size_t pAddress ) {

		switch( pAddress & 0x0F ) {
				
			case 0x00:		// Port B. 
						
				/*    Bits #0-#1: Head step direction. Decrease value (%00-%11-%10-%01-%00...) to move head downwards; increase value (%00-%01-%10-%11-%00...) to move head upwards.
					Bit #2: Motor control; 0 = Off; 1 = On.
					Bit #3: LED control; 0 = Off; 1 = On.
					Bit #4: Write protect photocell status; 0 = Write protect tab covered, disk protected; 1 = Tab uncovered, disk not protected.
					Bits #5-#6: Data density; %00 = Lowest; %11 = Highest.
					Bit #7: 0 = Data bytes are being currently read from disk; 1 = SYNC marks are being read.
				*/
				return mPortB->valueGet();

			case 0x01:		// Port A. Data byte last read from or to be next written onto disk.
				return 0;

			case 0x0c:
				return mAuxControlReg;
				
		}

		return cVia_Mos_6522::busReadByte( pAddress );
	}

	virtual void		 busWriteByte( size_t pAddress, byte pData ) {
		
		switch( pAddress & 0x0F ) {
            case 0x00:		// Port B. 
                // Bit 0: Schrittmotor Spule 0
                // Bit 1: Schrittmotor Spule 1
                // Bit 2: 1 = Laufwerksmotor an
                // Bit 3: 1 = rote LED an
                // Bit 4: 0 = Diskette schreibgeschützt
                // Bit 5,6: Timersteuerung
                // Bit 7: 0 = SYNC-Signal
                    if ((mPortB->valueGet() & 0x03) != (pData & 0x03)) {
                        // Bits #0-#1: Head step direction. 
                        // Decrease value (%00-%11-%10-%01-%00...) to move head downwards; 
                        // Increase value (%00-%01-%10-%11-%00...) to move head upwards.
                        if ((pData & 0x03) == ((mPortB->valueGet()+1) & 0x03)) {
                                // Move head upwards...
                                ((cDisk_Drive*)mParent)->headMove(1);
                        } else if ((pData & 0x03) == ((mPortB->valueGet()-1) & 0x03)) {
                                // Move head downwards...
                                ((cDisk_Drive*)mParent)->headMove(-1);
                        } else {
                                // warn("Unexpected stepper motor control sequence in VC1541 detected\n");
                        }
                    }

                    if ((mPortB->valueGet() & 0x04) != (pData & 0x04)) {
                    if (pData & 0x04)
                        ((cDisk_Drive*)mParent)->diskRotateStart();
                    else 
                        ((cDisk_Drive*)mParent)->diskRotateStop();
                    }

                    if ((mPortB->valueGet() & 0x08) != (pData & 0x08)) {
                        if (pData & 0x08)
                        ((cDisk_Drive*)mParent)->LED("Red", true);
                    else 
                        ((cDisk_Drive*)mParent)->LED("Red", false);
                    }

                    mPortB->valueSet(pData);
  
                return;

		    case 0x01: 
                        // Port A: Daten vom/zum Tonkopf
                       // if (tracingEnabled()) {
                        //        debug(1, " W%02X", value);
                        //}
                        mPortA->valueSet( pData );
                        return;

                case 0x03:
                        mDataDirectionPortA = pData;
                        if (mDataDirectionPortA != 0x00 && mDataDirectionPortA != 0xFF) {
                                //debug(1, "Data direction bits of VC1541 contain suspicious values\n");
                        }
                        return;
                        
                case 0x0C:
                        // Auxiliary control register
                        if ((pData & 0x02) != (mAuxControlReg & 0x02)) {
                                // debug("%s V-flag to drive head\n", value & 0x02 ? "Attach" : "Detach");
                        }
                        if ((pData & 0x20) != (mAuxControlReg & 0x20)) {
                               // debug(2, "Switching to %s mode (%04X) ByteReadyTimer = %d\n", value & 0x20 ? "READ" : "WRITE", floppy->cpu->getPC(),floppy->byteReadyTimer);
                        }
                        mAuxControlReg = pData;
                        return;

		}

		return cVia_Mos_6522::busWriteByte( pAddress, pData );
	}
};
