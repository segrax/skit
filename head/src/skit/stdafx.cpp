#include "stdafx.hpp"
#include "sepr/sepr.hpp"
#include "sepr/device/device.hpp"
#include "sepr/device/deviceConnection.hpp"
#include "sepr/systems/system.hpp"

int main() {

	cSepr *sepr = new cSepr("Commodore64");

	if( !sepr->systemPrepare() )
		return -1;

	cSystem *system = sepr->mSystemGet();

	system->mCyclesRemainingAdd(1);

	bool step = false;

	sepr->mSystemGet()->mDebugGet()->device( eDebug_Message, sepr, sepr->mSystemGet(), "Starting\n" );

	while( !system->mQuitThreadGet() ) {

		if( system->mCyclesRemainingGet() == 0 ) {
			if(!step) {
				system->mCyclesRemainingAdd( 10 );
				continue;
			}

			system->mDebugGet()->waitUser( system, "System Step" );
			system->mCyclesRemainingAdd( 1 );
		}

		Sleep(100);

	}
}
