
class cDrive : public cSystem {
protected:
	std::map<std::string, bool> mLEDs;

public:
							 cDrive( std::string pName, cSepr *pSepr ) : cSystem( pName, pSepr ) { }
	
	virtual size_t			 cycle() = 0;

	virtual SDL_Surface		*videoGet() { return 0; }

	void					 LED( std::string pName, bool pOn ) {
		std::map<std::string, bool>::iterator ledIT = mLEDs.find( pName );

		if( ledIT == mLEDs.end() )
			return;

		ledIT->second = pOn;
	}

	void					 LED_Add( std::string pName ) {

		mLEDs.insert( make_pair( pName, false ) );
	}
};
