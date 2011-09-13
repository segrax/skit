template <class tType> class cPort : public cDevice {

	tType					 mValue;

public:

							 cPort( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent ) : cDevice( pName, pSepr, pSystem, pParent) { 
                                 mValue = 0; 
                             }

	virtual	tType			 valueGet()										{ return mValue; }
	virtual void			 valueSet( tType pValue )	                    { mValue = pValue; }

	void					 bitSet( size_t pBitNumber, bool pOn ) {

		if(!pOn)
			mValue = (mValue & (~pBitNumber));
		else
			mValue |= (pBitNumber << 1);

	}

    virtual size_t             cycle() {

        return 1;
    }
};
