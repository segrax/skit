enum eChip_Data_Type {
	eData_Type_Byte		= 0,
	eData_Type_Word		= 1,
	eData_Type_DWord	= 2,
	eData_Type_Flag		= 3,
	eData_Type_Flags	= 4,
};

class cChip_Registers;
class cCpu;

class cChip_Register {
private:
	std::string					 mName;
	eChip_Data_Type				 mType;

protected:
	cCpu						*mCpu;

public:
						 cChip_Register( cCpu *pCpu, std::string pName, eChip_Data_Type pType ) : mName( pName ), mType( pType ), mCpu(pCpu) { }

	inline std::string	 mNameGet() { return mName; }
};

// Class which holds ptrs to all the CPU Registers
class cChip_Registers {
private:
	std::map< std::string, cChip_Register* >			mRegisters;

public:
									
									~cChip_Registers();

	void							 add( cChip_Register *pRegister );
	cChip_Register					*get( std::string pName );
};

template <class tSize> class cChip_Register_Value : public cChip_Register {
protected:
	tSize				 mData;
	bool				 mFlagSet;

public:
						 cChip_Register_Value( cCpu *pCpu, std::string pName, eChip_Data_Type pType, tSize pVal, bool pFlagSet ) : cChip_Register( pCpu, pName, pType ) { 
							 mFlagSet = pFlagSet;
							 mData = pVal; 
						 }

		void			 flagSet() {
			if(mFlagSet)
				mCpu->registerFlagSet( mData );
		}

		inline tSize	 operator()()				{ return mData;   }					// Return value

		inline void		 operator*( tSize pVal )	{ mData = pVal; }					// Set value, but don't adjust flags
		//inline void		 operator=( tSize &pVal )	{ mData = pVal; flagSet(); }		// Set Value, adjust flags (if required)

		inline tSize	 operator++()				{ return mData++;	   }
		inline tSize	 operator++(int)			{ size_t ret = mData; ++mData; flagSet(); return ret; }		// Return value, and increase

		inline tSize	 operator--()				{ return mData--;	   }
		inline tSize	 operator--(int)			{ size_t ret = mData; --mData; flagSet(); return ret; }

		inline tSize	 operator+=(tSize pVal)		{ tSize tmp = mData; mData += pVal; flagSet(); return tmp; }

		inline tSize	 operator&=(tSize pVal)		{ mData &= pVal; flagSet(); return mData; }
		inline tSize	 operator|=(tSize pVal)		{ mData |= pVal; flagSet(); return mData; }

		inline tSize	 operator>=(tSize pVal)		{	return cmpSet( mData ); }
		
};

class cChip_Register_Byte : public cChip_Register_Value<byte> {
private:

public:
						 cChip_Register_Byte(cCpu *pCpu, std::string pName, byte pValue, bool pFlagSet) : cChip_Register_Value<byte>( pCpu, pName, eData_Type_Byte, pValue, pFlagSet ) {

						 }

	inline void		 operator=( byte pVal )	{ mData = pVal; flagSet(); }		// Set Value, adjust flags (if required)

};

class cChip_Register_Word : public cChip_Register_Value<word> {
private:

public:
						 cChip_Register_Word(cCpu *pCpu, std::string pName, word pValue, bool pFlagSet) : cChip_Register_Value( pCpu, pName, eData_Type_Word, pValue, pFlagSet ) {
						
						 }

	inline void		 operator=( word pVal )	{ mData = pVal; flagSet(); }		// Set Value, adjust flags (if required)

};

class cChip_Register_Flag;

class cChip_Register_Flags : public cChip_Register {
private:
	std::map< std::string, cChip_Register_Flag* >			mRegisters;

public:
								 	 cChip_Register_Flags(cCpu *pCpu, std::string pName ) : cChip_Register( pCpu, pName, eData_Type_Flags ) { }
									~cChip_Register_Flags();

	void							 add( cChip_Register_Flag *pRegister );
	cChip_Register_Flag				*get( std::string pName );


};

class cChip_Register_Flag : public cChip_Register {
protected:
	bool					 mSet;
	size_t					 mValue;
	cChip_Register_Flags	*mFlags;

public:
						 cChip_Register_Flag(cCpu *pCpu, cChip_Register_Flags *pFlags, std::string pName, size_t pValue ) : cChip_Register( pCpu, pName, eData_Type_Flag ) {
							 mSet = false;
							 mValue = pValue;
							 mFlags = pFlags;
						 }

	bool				 get()				{ return mSet; }
	bool				 operator=( bool pVal ) { mSet = pVal; return mSet; }
	bool				 operator==(bool pVal ) { return pVal == mSet; }
};
