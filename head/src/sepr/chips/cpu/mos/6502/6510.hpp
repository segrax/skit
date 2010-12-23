
class cCpu_Mos_6510 : public cCpu_Mos_6502 {
protected:
	byte					 mProcessorPort[2];

public:
							 cCpu_Mos_6510( std::string pName, cSepr *pSepr, cDevice *pParent );
	
	virtual byte			 busReadByte( size_t pAddress );
	virtual void			 busWriteByte( size_t pAddress, byte pData );
	virtual void			 busWriteWordLE( size_t pAddress, word pData );

	void					 reset();

	byte					 mProcessorPortGet( size_t pIndex ) { return mProcessorPort[pIndex]; }
};
