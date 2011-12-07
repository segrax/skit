class cCommodore_64_Serial : public cPort<byte> {
    cVia1_SerialPort    *mConnectedDevice;
    byte                 mData, mDataDirection;

protected:

public:
						 cCommodore_64_Serial( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent, cVia1_SerialPort *pDevice );
	
	virtual byte		 busReadByte( size_t pAddress );
	virtual void		 busWriteByte( size_t pAddress, byte pData );

    inline byte          DataDirectionGet() { return mDataDirection; }
};