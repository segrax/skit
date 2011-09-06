class cCpu_Mos_6510;
class cChip_Rom;

#include "chips/interfaceAdapter/mos/6522.hpp"

class cSerialPortA : public cVia_Mos_6522, public cPort<byte> {


public:
	cSerialPortA(cSepr *pSepr, cSystem *pSystem, cDevice *pDevice ) : cVia_Mos_6522( "VIA1", pSepr, pSystem, pDevice ) {

	}

	virtual byte		 busReadByte( size_t pAddress ) {

		switch( pAddress ) {

			case 0x00:
				return valueGet();

			case 0x01:

			case 0x02:
				break;
		}

		return 0;
	}

	virtual void		 busWriteByte( size_t pAddress, byte pData ) {

		switch( pAddress & 0x0F ) {

			case 0x00:
				return valueSet( pData );
		
			case 0x01:

			case 0x02:
				break;
		}

		return;
	}
};

class cSerialPortB : public cVia_Mos_6522 {


public:
	cSerialPortB(cSepr *pSepr, cSystem *pSystem, cDevice *pDevice ) : cVia_Mos_6522( "VIA2", pSepr, pSystem, pDevice ) {

	}

	virtual byte		 busReadByte( size_t pAddress ) {
		
		return 0;
	}

	virtual void		 busWriteByte( size_t pAddress, byte pData ) {

		return;
	}
};

class cDrive_Commodore_1541_II : public cDrive {
protected:
	cCpu_Mos_6502			*mCpu;
	cChip_Rom				*mRom;
	cChip_Ram				*mRam;
	cSerialPortA			*mSerialA;
	cSerialPortB			*mSerialB;

public:

							 cDrive_Commodore_1541_II( std::string pName, cSepr *pSepr);
	
	virtual byte			 busReadByte( size_t pAddress );
	virtual word			 busReadWordLE( size_t pAddress );
	virtual void			 busWriteByte( size_t pAddress, byte pData );
	virtual void			 busWriteWordLE( size_t pAddress, word pData );
	
	cDevice					*deviceGet( size_t pAddress, bool pRead );
	
	void					 interruptAdd( std::string pName, cDevice *pDevice );
	void					 interruptRemove( std::string pName );

	size_t					 cycle();
	void					 reset();
	bool					 prepare();

	cPort<byte>				*serialPortAGet() { return mSerialA; }
};
