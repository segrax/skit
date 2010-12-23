class cChip_Rom;
class cChip_Ram;
class cCpu_Mos_6510;
class cVideo_Mos_8567;
class cVideoWindow;
class cCia_Mos_6526;

class cSystem_Commodore_64 : public cSystem {
private:
	cChip_Ram					*mRam,	 *mRamColor;
	cChip_Rom					*mBasic, *mKernal, *mChar;
	cCpu_Mos_6510				*mCpu;
	cVideo_Mos_8567				*mVideo;
	cCia_Mos_6526				*mCia1, *mCia2;

protected:
	size_t						 cycle();

public:

								 cSystem_Commodore_64( cSepr *pSepr );
								~cSystem_Commodore_64();

	virtual word				 deviceReadWord( cVideo_Mos_8567 *pVic, size_t pAddress ) ;
	virtual byte				 deviceReadByte( cDevice *pFromDevice, size_t pAddress );

	virtual byte				 busReadByte( size_t pAddress );
	virtual word				 busReadWordLE( size_t pAddress );
	virtual void				 busWriteByte( size_t pAddress, byte pData );
	virtual void				 busWriteWordLE( size_t pAddress, word pData );

	cDevice						*busVicGet(	size_t pAddress, bool pRead );

	cDevice						*deviceGet( size_t pAddress, bool pRead );
	cDevice						*deviceIOGet( size_t pAddress, bool pRead );
	
	void						 interruptAdd( std::string pName, cDevice *pDevice );
	void						 interruptRemove( std::string pName );

	bool						 prepare();
	SDL_Surface					*videoGet();

};
