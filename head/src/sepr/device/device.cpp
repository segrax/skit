#include "../sepr.hpp"
#include "deviceConnection.hpp"
#include "device.hpp"

void *startDevice( void *pDevice ) {

	((cDevice*) pDevice)->thread();

	return 0;
}

cDevice::cDevice( std::string pName, cSepr *pSepr, cDevice *pParent ) {

	mAnalyse = false;

	//
	mCycling = false;

	mCycle = 0;
	mCyclesTotal = 0;
	mCyclesRemaining = 0;
	mName = pName;

	//
	mSleepTime = 0;
	mQuitThread = false;

	//
	mSepr = pSepr;

	// Create the thread
	mCycleThread = 0;

	// Init the mutex
	pthread_mutex_init( &mCycleThreadMutex , 0 );

}

cDevice::~cDevice(void) {
	std::map< std::string, cDeviceConnection*>::iterator	 conIT;

	for( conIT = mConnections.begin(); conIT != mConnections.end(); ++conIT ) {
		delete conIT->second->mDeviceBGet();
		delete conIT->second;
	}

	delete mCycleThread;

	pthread_mutex_destroy(&mCycleThreadMutex);
}

bool cDevice::deviceConnect( cDevice *pDevice, size_t pAddress, size_t pSize ) {

	if( mConnections.find( pDevice->mNameGet() ) != mConnections.end() )
		return false;

	cDeviceConnection *connection = new cDeviceConnection(this, pDevice, pAddress, pSize );

	mConnections.insert( make_pair( pDevice->mNameGet(), connection ) );
	return true;
}

cDevice	*cDevice::deviceGet( std::string pName, bool pRead ) {
	
	if( mConnections.find( pName ) == mConnections.end() )
		return 0;

	return mConnections[ pName ]->mDeviceBGet();
}

cDevice *cDevice::deviceGet( size_t pAddress, bool pRead ) {
	std::map< std::string, cDeviceConnection*>::iterator	 conIT;
		
	for( conIT = mConnections.begin(); conIT != mConnections.end(); ++conIT ) {
		if( conIT->second->isInRange( pAddress ) )
			return conIT->second->mDeviceBGet();
	}

	return 0;
}

void cDevice::mCyclesRemainingAdd( size_t pVal ) {
	pthread_mutex_lock( &mCycleThreadMutex );
	mCyclesRemaining += pVal;
	pthread_mutex_unlock( &mCycleThreadMutex );
}

void cDevice::thread() {
	
	while(!mQuitThread) {

		if( mCyclesRemaining ) {

			pthread_mutex_lock( &mCycleThreadMutex );
			mCycling = true;

			cycle();
			mCycling = false;
			pthread_mutex_unlock( &mCycleThreadMutex );

		} else
			Sleep(mSleepTime);
	}
}

void cDevice::threadStart() {
	pthread_attr_t	threadAttr;
    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

	// Create the thread
	mCycleThread = new pthread_t();

	// Start the thread
	pthread_create(mCycleThread   , &threadAttr, &startDevice,   (void *) this );
}

cSystem *cDevice::mSystem() {
	return mSepr->mSystemGet();
}

byte cDevice::busReadByte( size_t pAddress ) {
	return 0;
}

word cDevice::busReadWordLE( size_t pAddress ) {
	return 0;
}

void cDevice::busWriteByte( size_t pAddress, byte pData ) {

}

void cDevice::busWriteWordLE( size_t pAddress, word pData ) {

}

void cDevice::mAnalyseSet( bool pVal ) {
	mAnalyse = pVal;

	std::map< std::string, cDeviceConnection*>::iterator	 conIT;
		
	for( conIT = mConnections.begin(); conIT != mConnections.end(); ++conIT )
		conIT->second->mDeviceBGet()->mAnalyseSet( pVal );

}
