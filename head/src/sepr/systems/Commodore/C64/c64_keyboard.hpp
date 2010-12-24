class cCommodore_64_Keyboard : public cCia_Mos_6526, public cKeyboard {
	byte				 mKeysRow[8], mKeysCol[8];

	size_t				 SDLKeyToC64( size_t pKey);

public:
						 cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cDevice *pParent );
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );
	
	virtual void		 releaseKey( size_t pKey );
	virtual void		 pressKey( size_t pKey );


};