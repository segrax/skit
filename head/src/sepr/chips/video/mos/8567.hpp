
class cVideo_Mos_8567 : public cVideo { 
private:
protected:
	word				 mRegMobX[8];										// Movable Object Block X-Coords
	byte				 mRegMobY[8];										// Movable Object Block Y-Coords
	byte				 mRegMobColor[8];

	byte				 mRegMobXBit8;
	byte				 mRegMobEnabled;									// MOBs Enabled (1 bit per MOB)
	byte				 mRegMobYExpansion;								// MOB Y Expansion
	byte				 mRegMobDataPriority;								//
	byte				 mRegMobMultiColor;								// MOB is multicolor
	byte				 mRegMobXExpansion;								// 
	byte				 mRegMobCollisionMob, mRegMobCollisionData;			// MOB-MOB Collision / MOB-Data Collision
	byte				 mRegMobMultiColors[2];
			
	byte				 mRegInterrupt, mRegInterruptEnabled;				// Interrupt Register / Enable Bits

	byte				 mRegControl1, mRegControl2;						// Control Registers
	byte				 mRegMemoryPtrs;									// Memory Pointers
	word				 mRegRasterY, mRegRasterInterruptY;					// Raster Registers
	byte				 mRegRowCounter;

	byte				 mRegBorderColor;									// Color to draw border
	byte				 mRegBackgroundColor[4];							// Color to draw background
		
	word				 mVidBaseSrc, mVidBaseChar, mVidBaseBitmap;
	word				 mVidSrc,	  mVidChar,		mVidBitmap;

	int					 mDrawY;

	void				 decode_StandardText();

	void				 interruptRasterFire();

public:
						 cVideo_Mos_8567( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent );
						~cVideo_Mos_8567();

	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );

	size_t				 cycle();
	void				 reset();


	void				 paletteLoad();
};
