class cChip_Ram : public cChip_Rom {

private:

public:
						 cChip_Ram( std::string pName, cSepr *pSepr, cDevice *pParent, size_t pSize );

	virtual void		 busWriteByte( size_t pAddress, byte pData );
	virtual void		 busWriteWordLE( size_t pAddress, word pData );
};
