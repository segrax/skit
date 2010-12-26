class cDevice;

template <class tType> class cConnection {
protected:
	tType				 mPins;
	cDevice				*mDevice1, *mDevice2;

public:
						 cConnection( cDevice *pDevice1, cDevice *pDevice2 );

	inline tType		 mPinsGet()				 { return mPins; }
	inline void			 mPinsSet( tType pPins ) { mPins = pPins; }

}

