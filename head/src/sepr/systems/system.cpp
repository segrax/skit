#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "system.hpp"

cSystem::cSystem( std::string pName, cSepr *pSepr ) : cDevice( pName, pSepr, 0 ) {
	mDebug = new cDebug( pSepr );
}

cSystem::~cSystem() {
	delete mDebug;
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
