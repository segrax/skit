
class cSkit {
private:
	cSepr			*mSepr;
	cSystem			*mSystem;

	SDL_Event		 mEvent;


public:
					 cSkit();
					~cSkit();

	bool			 systemStart( std::string pSystemName );
					 
};
