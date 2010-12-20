class cChip_Rom;
class cChip_Ram;
class cCpu_Mos_6510;
class cVideo_Mos_8567;
class cVideoWindow;

class cSystem_Commodore_64 : public cSystem {
private:
	cChip_Ram					*mRam;
	cChip_Rom					*mBasic, *mKernal, *mChar;
	cCpu_Mos_6510				*mCpu;
	cVideo_Mos_8567				*mVideo;

protected:
	void						 cycle();

public:

								 cSystem_Commodore_64( cSepr *pSepr );
								~cSystem_Commodore_64();

	virtual byte				 deviceReadByte( cDevice *pFromDevice, size_t pAddress );
	virtual byte				 busReadByte( size_t pAddress );
	virtual word				 busReadWordLE( size_t pAddress );
	virtual void				 busWriteByte( size_t pAddress, byte pData );
	virtual void				 busWriteWordLE( size_t pAddress, word pData );

	cDevice						*busVicGet(	size_t pAddress, bool pRead );

	cDevice						*deviceGet( size_t pAddress, bool pRead );
	cDevice						*deviceIOGet( size_t pAddress, bool pRead );

	bool						 prepare();
	SDL_Surface					*videoGet();

};
