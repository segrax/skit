#include <iomanip>

class cChip_Registers;
class cCpu;

class cChip_Register {
private:
	std::string					 mName;
    eBase_Data_Type              mType;
protected:
	cCpu						*mCpu;

public:
						 cChip_Register( cCpu *pCpu, eBase_Data_Type pType, std::string pName ) : mName( pName ), mType( pType), mCpu(pCpu) { }

	virtual std::string	 debug_CPU_Info_String() {

		return mName + ": ";
	}

	inline std::string	 mNameGet()		{ return mName; }
};

template <class tSize> class cChip_Register_Value : public cBase_Value<tSize>, public cChip_Register {
protected:
	bool				 mFlagSet;

public:
						 cChip_Register_Value( cCpu *pCpu, std::string pName, eBase_Data_Type pType, tSize pVal, bool pFlagSet ) : cChip_Register( pCpu, pType, pName ), cBase_Value( pType, pVal ) { 
							 mFlagSet = pFlagSet;
						 }

		void			 flagSet() {
			if(mFlagSet)
				mCpu->registerFlagSet( mData );
		}

        inline tSize     mDataGet()                { return mData; }
		inline tSize	 operator()()				{ return mData;   }					// Return value

		inline void		 operator*( tSize pVal )	{ mData = pVal; }					// Set value, but don't adjust flags
		//inline void		 operator=( tSize &pVal )	{ mData = pVal; flagSet(); }		// Set Value, adjust flags (if required)

		inline tSize	 operator++()				{ size_t ret = ++mData; flagSet(); return ret; }
		inline tSize	 operator++(int)			{ size_t ret = mData; ++mData; flagSet(); return ret; }		// Return value, and increase

		inline tSize	 operator--()				{ size_t ret = --mData; flagSet(); return ret;	   }
		inline tSize	 operator--(int)			{ size_t ret = mData; --mData; flagSet(); return ret; }

		inline tSize	 operator+=(tSize pVal)		{ tSize tmp = mData; mData += pVal; flagSet(); return tmp; }

		inline tSize	 operator&=(tSize pVal)		{ mData &= pVal; flagSet(); return mData; }
		inline tSize	 operator|=(tSize pVal)		{ mData |= pVal; flagSet(); return mData; }

		inline tSize	 operator^=(tSize pVal)		{ mData ^= pVal; flagSet(); return mData; }
		inline tSize	 operator<<=(tSize pVal)	{ mData <<= pVal; flagSet();return mData; }
		inline tSize	 operator>>=(tSize pVal)	{ mData >>= pVal; flagSet();return mData; }

		virtual std::string	 debug_CPU_Info_String() {
			size_t				value = mData;
			std::stringstream	msg;

			msg << cChip_Register::debug_CPU_Info_String();
			
			
			if( mNameGet() == "PC" )
				--value;

			msg << cBase_Value::debug_String();

			return msg.str();
		}
};

// Class which holds ptrs to all the CPU Registers
class cChip_Registers {
private:
	std::map< std::string, cChip_Register* >			mRegisters;

public:
									~cChip_Registers();

	void							 add( cChip_Register *pRegister );
	cChip_Register					*get( std::string pName );


    std::map< std::string, cChip_Register* > *getRegisters() { return &mRegisters; }
};

class cChip_Register_Byte : public cChip_Register_Value<byte> {
private:

public:
    cChip_Register_Byte(cCpu *pCpu, std::string pName, byte pValue, bool pFlagSet) : cChip_Register_Value( pCpu, pName, eData_Type_Byte, pValue, pFlagSet ) {

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
								 	 cChip_Register_Flags(cCpu *pCpu, std::string pName ) : cChip_Register( pCpu, eData_Type_Flags, pName ) { }
									~cChip_Register_Flags();

	void							 add( cChip_Register_Flag *pRegister );
	cChip_Register_Flag				*get( std::string pName );

	size_t							 value();
	void							 valueSet( size_t pNewValue );

	virtual std::string	 debug_CPU_Info_String() {
			std::stringstream msg;
			msg << cChip_Register::debug_CPU_Info_String();
			msg << "0x" << std::hex << (int) value();

			return msg.str();
		}
};

class cChip_Register_Flag : public cChip_Register {
protected:
	bool					 mSet;
	size_t					 mValue;
	cChip_Register_Flags	*mFlags;

public:
						 cChip_Register_Flag(cCpu *pCpu, cChip_Register_Flags *pFlags, std::string pName, size_t pValue ) : cChip_Register( pCpu, eData_Type_Flag, pName ) {
							 mSet = false;
							 mValue = pValue;
							 mFlags = pFlags;
						 }

	bool				 get()				{ return mSet; }
	int					 getInt()			{ return (mSet == true) ? 1 : 0; }

	bool				 operator=( bool pVal ) { mSet = pVal; return mSet; }
	bool				 operator==(bool pVal ) { return pVal == mSet; }

	inline size_t		 mValueGet()	{ return mValue; }

};
