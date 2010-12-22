
class cChip_Rom : public cDevice {
protected:
	byte				*mMemory;
	size_t				 mMemorySize;

	virtual size_t		 cycle();

public:
						 cChip_Rom( std::string pName, cSepr *pSepr, cDevice *pParent );
						~cChip_Rom();
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual word		 busReadWordLE( size_t pAddress );
	//virtual void		 busWriteByte( size_t pAddress, byte pData );
	//virtual void		 busWriteWordLE( size_t pAddress, word pData );

	bool				 loadFile( std::string pFile );				

};
