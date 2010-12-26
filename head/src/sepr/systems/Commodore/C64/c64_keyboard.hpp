class cCommodore_64_Keyboard : public cCia_Mos_6526, public cKeyboard {
	byte				 mKeysRow[8], mKeysCol[8];

	size_t				 SDLKeyToC64( SDL_keysym pKey );

public:
						 cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent );
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );
	
	virtual void		 releaseKey( SDL_keysym pKey );
	virtual void		 pressKey( SDL_keysym pKey );


};