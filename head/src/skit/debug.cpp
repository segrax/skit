#include "stdafx.hpp"
#include "sepr/sepr.hpp"
#include "sepr/device/device.hpp"

#include <iostream>

cDebug::cDebug() {

}

cDebug::~cDebug() {

}

void cDebug::device( cDevice *pDevice, std::string pMessage ) {
	
	std::cout << "[" << pDevice->mNameGet() << "]: ";
	std::cout << pMessage;

}
