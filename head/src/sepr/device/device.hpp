class cDeviceConnection;

class cDevice {
private:
	std::map< std::string, cDeviceConnection*>	 mConnections;								// Devices which are connected

	std::string							 mName;												// Name of the device

	bool								 mQuitThread, mCycling;								// Exit cycle thread
	
	pthread_t							*mCycleThread;
	pthread_mutex_t						 mCycleThreadMutex;

protected:
	size_t								 mCyclesRemaining;									// Cycles Requested to be executed before return

	bool								 mAnalyse;
	bool								 mDebug;

	size_t								 mCycle;											// Cycle Count
	size_t								 mCycles;											// Cycles to be executed for this instruction
	size_t								 mCyclesTotal;										// Total Cycle Count

	eDebug_Level						 mDebugLevel;

	size_t								 mSleepTime;										// Sleep Time between thread() loops, milliseconds

	cSepr								*mSepr;
	cSystem								*mSystem;
	cDevice								*mParent;

	virtual size_t						 cycle() = 0;										// Main device cycle
	
public:
										 cDevice( std::string pName, cSepr *pSepr, cSystem *pSystem, cDevice *pParent );	
										~cDevice(void);										
	
	virtual bool						 deviceConnect( cDevice *pDevice, size_t pAddress, size_t pSize );	// Connect a device
	
	template <class tDev>	tDev		*deviceFind( std::string pName, bool pRead ) {
	
		if( mConnections.find( pName ) == mConnections.end() )
			return 0;

		return dynamic_cast<tDev*>( mConnections[ pName ]->mDeviceBGet());
	}

	virtual cDevice						*deviceGetByName( std::string pName, bool pRead );									// Get a device pointer by name
	virtual cDevice						*deviceGet( size_t pAddress, bool pRead );

	void								 thread();											// Device Thread Loop
	void								 threadStart();

	virtual byte						 deviceReadByte( cDevice *pFromDevice, size_t pAddress);

	virtual byte						 busReadByte( size_t pAddress );
	virtual word						 busReadWordLE( size_t pAddress );
	virtual void						 busWriteByte( size_t pAddress, byte pData );
	virtual void						 busWriteWordLE( size_t pAddress, word pData );

	// Public Member Setters / Getters
	void								 mCyclesRemainingAdd( size_t pVal );
	inline size_t						 mCyclesRemainingGet()		  { return mCyclesRemaining; }	// Number of cycles remaining
	
    inline size_t                        mCycleGet() { return mCycle; }
    inline size_t                        mCyclesTotalGet() { return mCyclesTotal; }

	void								 mAnalyseSet( bool pVal );

	inline std::string					 mNameGet()					  { return mName; }				// Get the device name
	inline void							 mNameSet( std::string pVal ) { mName = pVal; }				// Set the device name
	
    template< class tSystem > tSystem	*mSystemGet() { return dynamic_cast<tSystem*>( mSystem ); }
	inline void							 mSleepTimeSet( size_t pVal ) { mSleepTime = pVal; }

	inline bool							 mQuitThreadGet()			  { return mQuitThread; }
	inline void							 mQuitThreadSet()			  { mQuitThread = true; }		// Exit main thread loop

	inline bool							 mDebugGet()			{ return mDebug; }
	void								 mDebugSet( bool pVal, eDebug_Level pLevel, bool pChildren = true );
};
