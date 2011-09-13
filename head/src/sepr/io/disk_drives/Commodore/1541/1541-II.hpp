class cCpu_Mos_6510;
class cChip_Rom;

#include "chips/interfaceAdapter/mos/6522.hpp"
#include "via1.hpp"
#include "via2.hpp"

class cDrive_Commodore_1541_II : public cDisk_Drive {
protected:
	cCpu_Mos_6502			*mCpu;
	cChip_Rom				*mRom;
	cChip_Ram				*mRam;
	cVia1					*mVia1;
	cVia2					*mVia2;

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

	void					 headMove(int pDirection);
	void					 diskRotateStart();
	void					 diskRotateStop();

    inline cVia1            *via1Get() { return mVia1; }
    inline cVia2            *via2Get() { return mVia2; }
};
