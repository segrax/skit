#include "types.hpp"
#include "file.hpp"

byte *local_FileRead( std::string pFile, size_t	&pFileSize ) {
	std::ifstream		*fileStream;
	byte				*fileBuffer = 0;

	// Attempt to open the file
	fileStream = new std::ifstream ( pFile.c_str(), std::ios::binary );
	if(fileStream->is_open() != false) {
	
		// Get file size
		fileStream->seekg(0, std::ios::end );
		pFileSize = fileStream->tellg();
		fileStream->seekg( std::ios::beg );

		// Allocate buffer, and read the file into it
		fileBuffer = new byte[ pFileSize ];
		if(fileStream->read( (char*) fileBuffer, pFileSize ) == false) {
			delete fileBuffer;
			fileBuffer = 0;
		}
	}

	// Close the stream
	fileStream->close();
	delete fileStream;

	// All done ;)
	return fileBuffer;
}