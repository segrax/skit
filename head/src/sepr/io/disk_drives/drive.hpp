
class cDrive : public cSystem {
protected:

public:
					cDrive( std::string pName, cSepr *pSepr ) : cSystem( pName, pSepr ) { }
	
	virtual size_t		 cycle() = 0;
};
