
struct sKey {
	SDL_keysym	 mKey;
	bool		 mPressed;

				 sKey() { mPressed = false; }

};

class cKeyboard : public cDevice {
protected:
	sKey				 mKeyPressed[ 0x100 ];

public:

						 cKeyboard( std::string pName, cSepr *pSepr, cDevice *pParent );
	
	virtual size_t		 cycle();

	virtual void		 releaseKey( SDL_keysym pKey );
	virtual void		 pressKey( SDL_keysym pKey );
	virtual bool		 isPressed( size_t pKey );

	void				 sdlEvent( SDL_KeyboardEvent *pEvent );
};
