class cCommodore_64_Keyboard : public cKeyboard {

public:
						 cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cDevice *pParent );

	virtual void		 releaseKey( size_t pKey );
	virtual void		 pressKey( size_t pKey );
};