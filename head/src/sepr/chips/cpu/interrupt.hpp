
class cInterrupt {
protected:
	cDevice			*mDevice;
	std::string		 mName;

public:

								 cInterrupt( std::string pName, cDevice *pDevice );

	inline std::string			 mNameGet() { return mName; }

};

class cInterrupt_NMI : public cInterrupt {
protected:
	size_t	test;
public:
								 cInterrupt_NMI( std::string pName, cDevice *pDevice ) : cInterrupt( pName, pDevice ) {
								 
								 };

};
