#include "stdafx.hpp"
#include "sepr/sepr.hpp"
#include "sepr/device/device.hpp"
#include "sepr/chips/register.hpp"
#include "sepr/chips/opcode.hpp"
#include "sepr/chips/cpu/cpu.hpp"
#include "sepr/systems/system.hpp"

#include <iostream>

cDebug::cDebug( cSepr *pSepr ) {
	mContinue = true;
	mLevel = eDebug_Warning;
	mSepr = pSepr;
}

cDebug::~cDebug() {

}

void cDebug::device( eDebug_Level pLevel, cSepr *pSepr, cDevice *pDevice, std::string pMessage ) {
	if( pMessage.size() == 0 )
		return;

	if( pLevel > mLevel && !pDevice->mDebugGet() )
		return;

	std::cout << "[" << pDevice->mNameGet() << "]: ";
	std::cout << pMessage;

	if( pDevice->mDebugGet() || pLevel <= eDebug_Stop )
		waitUser( pDevice );
}

void cDebug::waitUser( cDevice *pDevice ) {
	cCpu			*cpu = 0;
	std::string		 input;

	mContinue = false;

	// Handle a CPU
	if( (cpu = dynamic_cast< cCpu* >( pDevice )) ) {
		
		while( !mContinue ) {

			input.clear();
			std::getline(std::cin, input);
			
			// No Previous, so execute previous command
			if( input == "" ) {
				input = mCommand;
			}

			// Step the cpu instruction worth of cycles
			if( input == "t" ) {
				mContinue = true;
				mSepr->mSystemGet()->mCyclesRemainingAdd( cpu->mOpcodeCurrentGet()->mCycles );
			}

			// Resume execution
			if( input == "x" ) {
				mContinue = true;
				pDevice->mDebugSet(false);
			}
		}

		mCommand = input;
		return;
	}

	while( !mContinue ) {

		input.clear();
		std::getline(std::cin, input);

		if( input == "s" )
			mContinue = true;
	}
}

void cDebug::waitUser( cDevice *pDevice, std::string pMessage ) {

	std::cout << pMessage << "\n";

	waitUser( pDevice );
}