#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/video/video.hpp"
#include "8567.hpp"

cVideo_Mos_8567::cVideo_Mos_8567( std::string pName, cSepr *pSepr, cDevice *pParent ) : cVideo("VIC-II", pSepr, pParent, 320, 200, 1 ) {
	
	mScaleSet(2);
}

cVideo_Mos_8567::~cVideo_Mos_8567() {
	
}

void cVideo_Mos_8567::cycle() {

}

void cVideo_Mos_8567::reset() {

}

void cVideo_Mos_8567::paletteLoad() {
	const byte C64pal[16][3] = {
		0x00, 0x00, 0x00,
		0xFF, 0xFF, 0xFF, 
		0x68, 0x37, 0x2b, 
		0x70, 0xa4, 0xb2, 
		0x6f, 0x3d, 0x86, 
		0x58, 0x8d, 0x43, 
		0x35, 0x28, 0x79, 
		0xb8, 0xc7, 0x6f, 
		0x6f, 0x4f, 0x25,
		0x43, 0x39, 0x00,
		0x9a, 0x67, 0x59,
		0x44, 0x44, 0x44,
		0x6c, 0x6c, 0x6c,
		0x9a, 0xd2, 0x84,
		0x6c, 0x5e, 0xb5,
		0x95, 0x95, 0x95
	};
	
	// Create the 32bit palette
	delete mPalette;
	mPaletteColors = 16;
	mPalette = new dword[ mPaletteColors ];

	// Temporary surface to convert colors
	SDL_Surface *surface = SDL_CreateRGBSurface(	SDL_SWSURFACE,	1,	1,	 32, 0, 0, 0, 0);

	// map each palette color to its sdl surface color
	for(int color=0; color < mPaletteColors; color++)
		mPalette[ color ] = SDL_MapRGB (	surface->format , C64pal[color][0], C64pal[color][1], C64pal[color][2] ) ;

	// cleanup
	SDL_FreeSurface( surface );
}
