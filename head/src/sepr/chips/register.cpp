#include "sepr.hpp"
#include "device/device.hpp"
#include "chips/register.hpp"
#include "chips/cpu/cpu.hpp"

cChip_Registers::~cChip_Registers() {
	std::map< std::string, cChip_Register* >::iterator		regIT;

	for( regIT = mRegisters.begin(); regIT != mRegisters.end(); ++regIT )
		delete regIT->second;
}

void cChip_Registers::add( cChip_Register *pRegister ) {
	
	mRegisters.insert( make_pair( pRegister->mNameGet(), pRegister ) );
}

cChip_Register *cChip_Registers::get( std::string pName ) {
	
	if( mRegisters.find( pName ) == mRegisters.end() )
		return 0;

	return mRegisters.find( pName )->second;
}


cChip_Register_Flags::~cChip_Register_Flags() {
	std::map< std::string, cChip_Register_Flag* >::iterator		regIT;

	for( regIT = mRegisters.begin(); regIT != mRegisters.end(); ++regIT )
		delete regIT->second;
}

void cChip_Register_Flags::add( cChip_Register_Flag *pRegister ) {
	
	mRegisters.insert( make_pair( pRegister->mNameGet(), pRegister ) );
}

cChip_Register_Flag *cChip_Register_Flags::get( std::string pName ) {
	
	if( mRegisters.find( pName ) == mRegisters.end() )
		return 0;

	return mRegisters.find( pName )->second;
}

size_t cChip_Register_Flags::value() { 
	size_t val = 0;
	std::map< std::string, cChip_Register_Flag* >::iterator		regIT;

	for( regIT = mRegisters.begin(); regIT != mRegisters.end(); ++regIT ) {

		// if flag is enabled, OR it into the final value
		if(regIT->second->get())
			val |= regIT->second->mValueGet();
	}

	return val;
}