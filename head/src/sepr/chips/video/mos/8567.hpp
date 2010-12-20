
class cVideo_Mos_8567 : public cVideo { 
private:
protected:
	word				 _rMobX[8];										// Movable Object Block X-Coords
	byte				 _rMobY[8];										// Movable Object Block Y-Coords
	byte				 _rMobColor[8];

	byte				 _rMobXBit8;
	byte				 _rMobEnabled;									// MOBs Enabled (1 bit per MOB)
	byte				 _rMobYExpansion;								// MOB Y Expansion
	byte				 _rMobDataPriority;								//
	byte				 _rMobMultiColor;								// MOB is multicolor
	byte				 _rMobXExpansion;								// 
	byte				 _rMobCollisionMob, _rMobCollisionData;			// MOB-MOB Collision / MOB-Data Collision
	byte				 _rMobMultiColors[2];
			
	byte				 _rInterrupt, _rInterruptEnabled;				// Interrupt Register / Enable Bits

	byte				 _rControl1, _rControl2;						// Control Registers
	byte				 _rMemoryPtrs;									// Memory Pointers
	word				 _rRasterY, _rRasterInterruptY;					// Raster Registers
	byte				 _rRowCounter;

	byte				 _rBorderColor;									// Color to draw border
	byte				 _rBackgroundColor[4];							// Color to draw background
		
	word				 _vidBaseSrc, _vidBaseChar, _vidBaseBitmap;
	word				 _vidSrc,	  _vidChar,		_vidBitmap;

public:
					 cVideo_Mos_8567( std::string pName, cSepr *pSepr, cDevice *pParent );
					~cVideo_Mos_8567();

	virtual byte	 busReadByte( size_t pAddress );
	virtual void	 busWriteByte( size_t pAddress, byte pData );

	void			 cycle();
	void			 reset();


	void			 paletteLoad();
};
