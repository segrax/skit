class cDevice;
class cSepr;

enum eDebug_Level {
	eDebug_Kill		= 0x00,
	eDebug_Stop		= 0x01,
	eDebug_Break	= 0x02,
	eDebug_Warning	= 0x03,
	eDebug_Message	= 0x04,

	eDebug_End		= 0x10
};

class cDebug {
private:
	bool				 mContinue;
	std::string			 mCommand;

	eDebug_Level		 mLevel;
	cSepr				*mSepr;

public:
				 cDebug( cSepr *pSepr );
				~cDebug();


	void		 device( eDebug_Level pLevel, cSepr *pSepr, cDevice *pDevice, std::string pMessage );

	void		 waitUser( cDevice *pDevice );
	void		 waitUser( cDevice *pDevice, std::string pMessage );
};
