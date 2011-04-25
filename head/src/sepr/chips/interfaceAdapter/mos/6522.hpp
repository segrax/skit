
class cVia_Mos_6522 : public cDevice {
protected:

public:

						 cVia_Mos_6522( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pPart );
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );

};
