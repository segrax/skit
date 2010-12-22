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

	memset( mBuffer, 0, mBufferSize );

	mWindow = 0;
	mSDLSurface = 0;
}

cVideo::~cVideo() {

	delete[] mBuffer;
	delete[] mPalette;

	SDL_FreeSurface( mSDLSurface );
}

bool cVideo::windowSet( cVideoWindow *pWindow ) {

	mWindow = pWindow;
	mWindow->titleSet( mSepr->mNameGet() );

	return true;
}	

size_t cVideo::cycle() {
	return 0;
}

SDL_Surface	*cVideo::surfaceGet() {
	if(!mSDLSurface)
		mSDLSurface = SDL_CreateRGBSurface( SDL_SWSURFACE, mWidth, mHeight, 32, 0, 0, 0, 0); 

	dword *dst = (dword*) mSDLSurface->pixels;
	byte  *src = mBuffer;

	if(!mPalette)
		return 0;

	for( size_t y = 0; y < mHeight; ++y )  {
		for( size_t x = 0; x < mWidth; ++x ) {

			*dst++ = mPalette[ *src++ ];
		}
	}

	return mSDLSurface;
}
