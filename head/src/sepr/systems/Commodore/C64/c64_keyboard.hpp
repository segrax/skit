class cCommodore_64_Keyboard : public cKeyboard {

public:
						 cCommodore_64_Keyboard( std::string pName, cSepr *pSepr, cDevice *pParent );

	virtual size_t		 cycle();

	size_t				 keyRow( size_t pKey, size_t pCol  );
};