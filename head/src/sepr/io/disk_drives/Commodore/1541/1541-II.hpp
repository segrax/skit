class cCpu_Mos_6510;
class cChip_Rom;

class cDrive_Commodore_1541_II : public cDrive {
protected:
	cCpu_Mos_6502			*mCpu;
	cChip_Rom				*mRom;
	cChip_Ram				*mRam;

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

	virtual SDL_Surface		*videoGet() { return 0; }
};
