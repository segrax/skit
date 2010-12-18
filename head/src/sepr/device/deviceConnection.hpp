class cDevice;

class cDeviceConnection {
private:
	cDevice								*mDeviceA, *mDeviceB;
	size_t								 mAddress, mSize;

public:
	// Constructor
	cDeviceConnection( cDevice *pDeviceA, cDevice *pDeviceB, size_t pAddress, size_t pSize ) { 
		mDeviceA = pDeviceA; mDeviceB = pDeviceB; mAddress = pAddress; mSize = pSize; }

										 ~cDeviceConnection() { }


	inline size_t						 mSizeGet()		{ return mSize; }
	inline size_t						 mAddressGet()  { return mAddress; }

	inline cDevice						*mDeviceAGet() { return mDeviceA; }
	inline cDevice						*mDeviceBGet() { return mDeviceB; }

	inline bool							 isInRange( size_t pAddress ) {
		if( pAddress >= mAddress && pAddress < mAddress + mSize )
			return true;
		return false;
	}

};
