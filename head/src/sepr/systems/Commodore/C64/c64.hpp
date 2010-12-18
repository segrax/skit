class cChip_Rom;
class cChip_Ram;
class cCpu_Mos_6510;

class cSystem_Commodore_64 : public cSystem {
private:
	cChip_Ram					*mRam;
	cChip_Rom					*mBasic, *mKernal, *mChar;
	cCpu_Mos_6510			*mCpu;

protected:
	void						 cycle();

public:

								 cSystem_Commodore_64( cSepr *pSepr );
								~cSystem_Commodore_64();
	
	virtual byte				 busReadByte( size_t pAddress );
	virtual word				 busReadWordLE( size_t pAddress );
	virtual void				 busWriteByte( size_t pAddress, byte pData );
	virtual void				 busWriteWordLE( size_t pAddress, word pData );

	cDevice						*deviceGet( size_t pAddress, bool pRead );
	cDevice						*deviceIOGet( size_t pAddress, bool pRead );

	bool						 prepare();
};
