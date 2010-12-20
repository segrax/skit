#include "common/scale2x/scalebit.hpp"

class cVideoWindow;

class cVideo : public cDevice {
protected:
	byte					*mBuffer;					// Screen Buffer
	dword					*mPalette;					// SDL Palette Colors

	bool					 mFullScreen;

	size_t					 mBufferSize, mPixelBytes, mPaletteColors;
	size_t					 mWidth, mHeight, mScale;	// Surface Details

	cVideoWindow			*mWindow;

protected:
	virtual void			 cycle();

public:
							 cVideo( std::string pName, cSepr *pSepr, cDevice *pParent, size_t pWidth, size_t pHeight, size_t pBytesPerPixel );
							~cVideo();
	
	virtual void			 paletteLoad() = 0;
	
	void					 mScaleSet( size_t pScale ) { mScale = pScale; }

	SDL_Surface				*surfaceGet();
	
	bool					 windowSet( cVideoWindow *pWindow );
};
