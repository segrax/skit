
class cVideo_Mos_8567 : public cVideo { 
private:

protected:

public:
					 cVideo_Mos_8567( std::string pName, cSepr *pSepr, cDevice *pParent );
					~cVideo_Mos_8567();


	void			 cycle();
	void			 reset();


	void			 paletteLoad();
};
