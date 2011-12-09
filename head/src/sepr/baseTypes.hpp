#include <iomanip>

enum eBase_Data_Type {
	eData_Type_Byte		= 0,
	eData_Type_Word		= 1,
	eData_Type_DWord	= 2,
	eData_Type_Flag		= 3,
	eData_Type_Flags	= 4,
};

template <class tSize> class cBase_Value {
protected:
    eBase_Data_Type      mDataType;
	tSize				 mData;

public:
						 cBase_Value( eBase_Data_Type pType, tSize pVal ) { 
							 mData = pVal;
                             mDataType =  pType;
						 }

                         virtual void mDataSet( tSize pValue ) {
                             mData = pValue;
                         }

		                virtual std::string	 debug_String() {
			                size_t				value = mData;
			                std::stringstream	msg;

			                msg << "0x" << std::hex;
			                msg << std::setfill('0');
			
                            switch( mDataType ) {
                                case eData_Type_Byte:
				                    msg << std::setw(2);
                                    break;

                                case eData_Type_Word:
				                    msg << std::setw(4);
                                    break;

			                    case eData_Type_DWord:
				                    msg << std::setw(8);
                                    break;
                            }
			                msg << value;

			                return msg.str();
		                }
};

class cBase_Byte : public cBase_Value<byte> {
private:

public:
						 cBase_Byte(byte pValue) : cBase_Value<byte>( eData_Type_Byte, pValue ) {

						 }

	inline void		 operator=( byte pVal )	{ mData = pVal; }		// Set Value, adjust flags (if required)

};

class cBase_Word : public cBase_Value<word> {
private:

public:
						 cBase_Word(word pValue) : cBase_Value( eData_Type_Word, pValue ) {
						
						 }

	inline void		 operator=( word pVal )	{ mData = pVal; }		// Set Value, adjust flags (if required)

};
