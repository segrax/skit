
class cVideoWindow {
	protected:
		SDL_Surface		*mSurface;
		size_t			 mWidth, mHeight;
		size_t			 mPixelBytes;
		bool			 mIsOpen;

	public:
						 cVideoWindow( size_t pWidth, size_t pHeight, size_t pBytesPixel, bool pFullScreen );
						~cVideoWindow();

		void			 clear( byte color );
		void			 blit( SDL_Surface *source, word x, word y, word pSourceX = 0, word pSourceY = 0);
		void			 titleSet( std::string pWindowTitle );

		inline bool		 isOpen() { return mIsOpen; }
	};
