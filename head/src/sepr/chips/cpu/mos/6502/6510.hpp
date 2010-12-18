
class cCpu_Mos_6510 : public cCpu_Mos_6502 {
protected:
	byte					 mProcessorPort;

public:
							 cCpu_Mos_6510( std::string pName, cSepr *pSepr, cDevice *pParent );

	void					 reset();

	byte					 mProcessorPortGet() { return mProcessorPort; }
};
