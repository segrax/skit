#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "system.hpp"
#include "analyse/analyse.hpp"

cSystem::cSystem( std::string pName, cSepr *pSepr ) : cDevice( pName, pSepr, 0, 0 ) {
	mDebug = new cDebug( pSepr );
    mAnalyse = new cAnalyse( pSepr );

	pthread_mutex_init( &mEventQueue , 0 );
}

cSystem::~cSystem() {
	delete mDebug;

	pthread_mutex_destroy(&mEventQueue);
}

std::string cSystem::systemDataPath( std::string pFile ) {
	std::stringstream		path;

	path << "data\\";
	path << mNameGet();

	path << "\\";
	path << pFile;

	return path.str();
}

size_t cSystem::cycle() {

	return 0;
}

void cSystem::eventAdd( SDL_Event *pEvent ) {
	pthread_mutex_lock( &mEventQueue );
	mEvents.push_back(*pEvent);
	pthread_mutex_unlock( &mEventQueue );
}
