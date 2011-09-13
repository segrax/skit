
class cVia1_SerialPort : public cPort<byte> {

    bool deviceAtnIsOutput, deviceClockIsOutput, deviceDataIsOutput, deviceAtnPin, deviceClockPin, deviceDataPin;
    bool ciaAtnIsOutput, ciaClockIsOutput, ciaDataIsOutput, ciaAtnPin, ciaClockPin, ciaDataPin;
    
    bool mLineAtn, mLineClock, mLineData;


protected:

    void        pinUpdate();

public:

	cVia1_SerialPort( cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cPort<byte>( "Via1_SerialPort", pSepr, pSystem, pParent ) {

        pinsCiaUpdate(0,0);
        pinsViaUpdate(0,0);

        mLineAtn = mLineClock = mLineData = false;
	}

	inline bool	lineClock()  { return mLineClock;  }
	inline bool lineData()   { return mLineData;   }
	inline bool lineAtn()    { return mLineAtn;    }

    void        pinsCiaUpdate( byte pValue, byte pDirection );          // Update the pins from the CIA
    void        pinsViaUpdate( byte pValue, byte pDirection );          // Update the pins from the drive

};

class cVia1 : public cVia_Mos_6522 {

public:
	cVia1(cSepr *pSepr, cSystem *pSystem, cDevice *pDevice );

    inline bool isAtnEnabled()  { return (mInterruptEnabled & 0x02) != 0; }
    inline void flagAtnSet()    { mInterruptFlags |= 0x02; }
	inline void flagAtnClear()  { mInterruptFlags &= ~0x02; }

	inline cVia1_SerialPort	*mPortBGet() { return (cVia1_SerialPort*) mPortB; }

	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );
};
