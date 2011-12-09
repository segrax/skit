#include <map>
#include <string>
#include <sstream>
#include <vector>

#ifndef _WIN32
	#include <unistd.h>
	#define Sleep( a ) usleep( a * 1000 );
#else
	#include <windows.h>
#endif 

#include <pthread.h>

#include "common/types.hpp"
#include "common/endian.hpp"
#include "baseTypes.hpp"

#include "skit/debug.hpp"

#include <SDL.h>

class cSystem;

class cSepr {
private:
	std::string					 mName;
	cSystem						*mSystem;
	
public:
								 cSepr( std::string pName );
								~cSepr();

	bool						 systemPrepare();

	cSystem						*mSystemGet() { return mSystem; }
	inline std::string			 mNameGet()	  { return mName; }

	
};
