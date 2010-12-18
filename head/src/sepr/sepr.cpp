#include "sepr.hpp"
#include "device\device.hpp"
#include "systems\systems.hpp"

cSepr::cSepr( std::string pName ) {
	mSystem = 0;

	mName = pName;
}

cSepr::~cSepr() {

	delete mSystem;
}

bool cSepr::systemPrepare() {
	if(mSystem)
		return false;

	// Available systems
	if( mName == "Commodore64" )
		mSystem = (cSystem*) new cSystem_Commodore_64( this );

	// 
	if( mSystem ) {

		// Prepare internals
		if( !mSystem->prepare() ) 
			return false;

		// Launch main thread
		mSystem->threadStart();
	}

	return true;
}
