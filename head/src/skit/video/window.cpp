#include "sepr/sepr.hpp"
#include "window.hpp"

cVideoWindow::cVideoWindow( size_t pWidth, size_t pHeight, size_t pBytesPixel, bool pFullScreen ) {
	size_t flags = SDL_ANYFORMAT;

	if( pFullScreen )
		flags |= SDL_FULLSCREEN;

	mWidth = pWidth;
	mHeight = pHeight;
	mPixelBytes = pBytesPixel;

	SDL_Init ( SDL_INIT_VIDEO ); 
	atexit ( SDL_Quit ) ;

	mSurface = SDL_SetVideoMode ( pWidth , pHeight , pBytesPixel * 8 , flags ) ;

	if(!mSurface) {
		mIsOpen = false;
	} else
		mIsOpen = true;
}

cVideoWindow::~cVideoWindow() {

	SDL_FreeSurface( mSurface );

}


void cVideoWindow::clear( byte color ) {
		
	SDL_FillRect( mSurface, NULL, color );
}

void cVideoWindow::blit( SDL_Surface *source, word x, word y, word pSourceX, word pSourceY ) {
	SDL_Rect srcRect, destRect;

	srcRect.x = pSourceX;
	srcRect.y = pSourceY;

	srcRect.h = source->h;
	srcRect.w = source->w;

	destRect.x = x;
	destRect.y = y;
	destRect.h = source->h;
	destRect.w = source->w;

	SDL_SetColorKey( source, SDL_SRCCOLORKEY, 0xFF);
	SDL_BlitSurface( source, &srcRect, mSurface, &destRect);

	SDL_UpdateRect(  mSurface, 0,0,0,0);
}

void cVideoWindow::titleSet( std::string pWindowTitle ) {
	SDL_WM_SetCaption( pWindowTitle.c_str(), 0 );
}
