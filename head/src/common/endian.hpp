#ifdef ENDIAN_BIG 
	// Read a word from the buffer
	inline word readWordLE( const void *buffer ) {
 		const byte *wordByte = (const byte *) buffer;
	
		return (wordByte[1] << 8) | wordByte[0];
	}
	
	inline void writeWordLE( const void *buffer, word pValue ) {
		byte *wordBytes = (byte *) buffer;

		wordBytes[0] = (pValue & 0x00FF);
		wordBytes[1] = (pValue & 0xFF00) >> 8;
	}

#else

	// Read a word from the buffer
	inline word readWordLE( const void *buffer ) {
		const word *wordBytes = (const word *) buffer;
		return *wordBytes;
	}

	inline void writeWordLE( const void *buffer, word pValue ) {
		word *wordBytes = (word *) buffer;
		*wordBytes = pValue;
	}	

#endif
