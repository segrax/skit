#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "video.hpp"
#include "skit/video/window.hpp"
#include "systems/system.hpp"

cVideo::cVideo( std::string pName, cSepr *pSepr, cDevice *pParent, 
				size_t pWidth, size_t pHeight, size_t pBytesPerPixel ) : cDevice( pName, pSepr, pParent ) {

	mFullScreen = false;

	mWidth = pWidth;
	mHeight = pHeight;

	mScale = 1;
	mPalette = 0;
	mPaletteColors = 0;
	
	mPixelBytes = pBytesPerPixel;

	mBufferSize = (pWidth * pHeight) * mPixelBytes;
	mBuffer = new byte[ mBufferSize ];

	mWindow = 0;
}

cVideo::~cVideo() {

	delete[] mBuffer;
	delete[] mPalette;
	delete mWindow;
}

bool cVideo::windowOpen( ) {

	delete mWindow;
	mWindow = new cVideoWindow( mWidth * mScale, mHeight * mScale, mPixelBytes, mFullScreen );
	if(!mWindow->isOpen()) 
		return false;

	mWindow->titleSet( mSepr->mNameGet() );

	return true;
}	

void cVideo::mScaleSet( size_t pScale ) {
	mScale = pScale;

	windowOpen();
}
