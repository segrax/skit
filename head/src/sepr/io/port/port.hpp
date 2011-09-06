template <class tType> class cPort {

	tType			 mValue;

public:

					 cPort() { mValue = 0; }

					 tType			 valueGet()					{ return mValue; }
					 void			 valueSet( tType pValue )	{ mValue = pValue; }

};
