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

	while( !system->mQuitThreadGet() ) {

		if( system->mCyclesRemainingGet() == 0 )
			system->mCyclesRemainingAdd(1);

		Sleep(100);

	}
}
