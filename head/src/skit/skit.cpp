#include "sepr/sepr.hpp"
#include "skit.hpp"
#include "sepr/device/device.hpp"
#include "sepr/device/deviceConnection.hpp"
#include "sepr/systems/system.hpp"
#include "video/window.hpp"

cSkit::cSkit() {

	mSepr = 0;

}


cSkit::~cSkit() {

	delete mSepr;
	delete mSystem;
}

bool cSkit::systemStart( std::string pSystemName ) {
	mSepr = new cSepr( pSystemName );

	if( !mSepr->systemPrepare() )
		return false;

	mSystem = mSepr->mSystemGet();

	mSystem->mCyclesRemainingAdd(1);

	bool step = false;

	// Output starting message
	mSepr->mSystemGet()->mDebugGet()->device( eDebug_Message,mSepr, mSepr->mSystemGet(), "Starting\n" );

	// Spin processing events until system end
	while( !mSystem->mQuitThreadGet() ) {

		SDL_PollEvent( &mEvent );

		if(mEvent.type == SDL_KEYDOWN || mEvent.type == SDL_KEYUP )
			mSystem->eventAdd( &mEvent );

		mSystem->mWindowGet()->clear(0);
		mSystem->mWindowGet()->blit( mSystem->videoGet(), 0, 0 );

		if( mSystem->mCyclesRemainingGet() == 0 ) {
			if(!step) {
				mSystem->mCyclesRemainingAdd( 19656 );
				continue;
			}

			mSystem->mDebugGet()->waitUser( mSystem, "System Step" );
			mSystem->mCyclesRemainingAdd( 1 );
		}

		Sleep(100);

	}

	return true;
}
