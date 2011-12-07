class cDebug;
class cVideoWindow;
class cAnalyse;

class cSystem : public cDevice {
private:
	cDebug					*mDebug;
    cAnalyse                *mAnalyse;

protected:
	std::vector<SDL_Event>	 mEvents;
	pthread_mutex_t			 mEventQueue;

	cVideoWindow			*mWindow;

	virtual size_t			 cycle();

public:
							 cSystem( std::string pName, cSepr *pSepr );
							~cSystem();
	
	virtual byte			 busReadByte( size_t pAddress ) = 0;
	virtual word			 busReadWordLE( size_t pAddress ) = 0;
	virtual void			 busWriteByte( size_t pAddress, byte pData ) = 0;
	virtual void			 busWriteWordLE( size_t pAddress, word pData ) = 0;

	virtual void			 eventAdd( SDL_Event *pEvent );
	virtual void			 interruptAdd( std::string pName, cDevice *pDevice ) = 0;
	virtual void			 interruptRemove( std::string pName ) = 0;

	virtual cDevice			*deviceGet( size_t pAddress, bool pRead ) = 0;

	virtual cDebug			*mDebugGet() { return mDebug; }

	virtual bool			 prepare() = 0;
	virtual SDL_Surface		*videoGet() = 0;

	std::string				 systemDataPath( std::string pFile );

	inline cVideoWindow			    *mWindowGet() { return mWindow; }

    inline cAnalyse                 *mAnalyseGet() { return mAnalyse; }

};
