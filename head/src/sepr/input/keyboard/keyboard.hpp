
class cKeyboard : public cDevice {
protected:
	bool				 mKeyPressed[ 0x100 ];

public:

						 cKeyboard( std::string pName, cSepr *pSepr, cDevice *pParent );
	
	virtual size_t		 cycle();

	virtual void		 releaseKey( size_t pKey );
	virtual void		 pressKey( size_t pKey );
	virtual bool		 isPressed( size_t pKey );

	void				 sdlEvent( SDL_KeyboardEvent *pEvent );
};
