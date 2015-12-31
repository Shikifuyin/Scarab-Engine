/*******************************************************************************/
/* /object/Wrappers.cpp - Internal Source **************************************/
/*******************************************************************************/
/* KOALIB Project by Ark *******************************************************/
/*******************************************************************************/

#include "Wrappers.hpp"


///////////////////////////////////////////////////////////////////////////////////
// Basic Types : UInt
	// constructors & destructor
UInt::UInt()
{
	m_Value = 0;
}
UInt::UInt(const unsigned int & rhs)
{
	m_Value = rhs;
}
UInt::UInt(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
UInt::UInt(const Char & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Byte & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Short & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const UShort & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Long & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const ULong & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Int & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const UInt & rhs)
{
	m_Value = rhs.m_Value;
}
UInt::UInt(const Long64 & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const ULong64 & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Float & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Double & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const LongDouble & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::UInt(const Pointer & rhs)
{
	m_Value = (unsigned int)(rhs.getValue());
}
UInt::~UInt()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Int
	// constructors & destructor
Int::Int()
{
	m_Value = 0;
}
Int::Int(const signed int & rhs)
{
	m_Value = rhs;
}
Int::Int(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
Int::Int(const Char & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Byte & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Short & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const UShort & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Long & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const ULong & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Int & rhs)
{
	m_Value = rhs.m_Value;
}
Int::Int(const UInt & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Long64 & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const ULong64 & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Float & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Double & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const LongDouble & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::Int(const Pointer & rhs)
{
	m_Value = (signed int)(rhs.getValue());
}
Int::~Int()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Bool
	// constructors & destructor
Bool::Bool()
{
	m_Value = false;
}
Bool::Bool(const bool & rhs)
{
	m_Value = rhs;
}
Bool::Bool(const Bool & rhs)
{
	m_Value = rhs.m_Value;
}
Bool::Bool(const Char & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const Byte & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const Short & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const UShort & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const Long & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const ULong & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const Int & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const UInt & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const Long64 & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const ULong64 & rhs)
{
	m_Value = (rhs.getValue() == 0) ? false : true;
}
Bool::Bool(const Float & rhs)
{
	m_Value = (rhs.getValue() == 0.0f) ? false : true;
}
Bool::Bool(const Double & rhs)
{
	m_Value = (rhs.getValue() == 0.0) ? false : true;
}
Bool::Bool(const LongDouble & rhs)
{
	m_Value = (rhs.getValue() == 0.0) ? false : true;
}
Bool::Bool(const Pointer & rhs)
{
	m_Value = (rhs.getValue() == NULL) ? false : true;
}
Bool::~Bool()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Char
	// constructors & destructor
Char::Char()
{
	m_Value = 0;
}
Char::Char(const signed char & rhs)
{
	m_Value = rhs;
}
Char::Char(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
Char::Char(const Char & rhs)
{
	m_Value = rhs.m_Value;
}
Char::Char(const Byte & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Short & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const UShort & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Long & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const ULong & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Int & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const UInt & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Long64 & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const ULong64 & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Float & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Double & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const LongDouble & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::Char(const Pointer & rhs)
{
	m_Value = (signed char)(rhs.getValue());
}
Char::~Char()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Byte
	// constructors & destructor
Byte::Byte()
{
	m_Value = 0;
}
Byte::Byte(const unsigned char & rhs)
{
	m_Value = rhs;
}
Byte::Byte(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
Byte::Byte(const Char & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Byte & rhs)
{
	m_Value = rhs.m_Value;
}
Byte::Byte(const Short & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const UShort & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Long & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const ULong & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Int & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const UInt & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Long64 & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const ULong64 & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Float & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Double & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const LongDouble & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::Byte(const Pointer & rhs)
{
	m_Value = (unsigned char)(rhs.getValue());
}
Byte::~Byte()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Short
	// constructors & destructor
Short::Short()
{
	m_Value = 0;
}
Short::Short(const signed short & rhs)
{
	m_Value = rhs;
}
Short::Short(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
Short::Short(const Char & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Byte & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Short & rhs)
{
	m_Value = rhs.m_Value;
}
Short::Short(const UShort & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Long & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const ULong & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Int & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const UInt & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Long64 & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const ULong64 & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Float & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Double & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const LongDouble & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::Short(const Pointer & rhs)
{
	m_Value = (signed short)(rhs.getValue());
}
Short::~Short()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : UShort
	// constructors & destructor
UShort::UShort()
{
	m_Value = 0;
}
UShort::UShort(const unsigned short & rhs)
{
	m_Value = rhs;
}
UShort::UShort(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
UShort::UShort(const Char & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Byte & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Short & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const UShort & rhs)
{
	m_Value = rhs.m_Value;
}
UShort::UShort(const Long & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const ULong & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Int & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const UInt & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Long64 & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const ULong64 & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Float & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Double & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const LongDouble & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::UShort(const Pointer & rhs)
{
	m_Value = (unsigned short)(rhs.getValue());
}
UShort::~UShort()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Long
	// constructors & destructor
Long::Long()
{
	m_Value = 0;
}
Long::Long(const signed long & rhs)
{
	m_Value = rhs;
}
Long::Long(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
Long::Long(const Char & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Byte & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Short & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const UShort & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Long & rhs)
{
	m_Value = rhs.m_Value;
}
Long::Long(const ULong & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Int & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const UInt & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Long64 & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const ULong64 & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Float & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Double & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const LongDouble & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::Long(const Pointer & rhs)
{
	m_Value = (signed long)(rhs.getValue());
}
Long::~Long()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : ULong
	// constructors & destructor
ULong::ULong()
{
	m_Value = 0;
}
ULong::ULong(const unsigned long & rhs)
{
	m_Value = rhs;
}
ULong::ULong(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
ULong::ULong(const Char & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Byte & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Short & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const UShort & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Long & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const ULong & rhs)
{
	m_Value = rhs.m_Value;
}
ULong::ULong(const Int & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const UInt & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Long64 & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const ULong64 & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Float & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Double & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const LongDouble & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::ULong(const Pointer & rhs)
{
	m_Value = (unsigned long)(rhs.getValue());
}
ULong::~ULong()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Long64
	// constructors & destructor
Long64::Long64()
{
	m_Value = 0;
}
Long64::Long64(const signed __int64 & rhs)
{
	m_Value = rhs;
}
Long64::Long64(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
Long64::Long64(const Char & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Byte & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Short & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const UShort & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Long & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const ULong & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Int & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const UInt & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Long64 & rhs)
{
	m_Value = rhs.m_Value;
}
Long64::Long64(const ULong64 & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Float & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Double & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const LongDouble & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::Long64(const Pointer & rhs)
{
	m_Value = (signed __int64)(rhs.getValue());
}
Long64::~Long64()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : ULong64
	// constructors & destructor
ULong64::ULong64()
{
	m_Value = 0;
}
ULong64::ULong64(const unsigned __int64 & rhs)
{
	m_Value = rhs;
}
ULong64::ULong64(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1 : 0;
}
ULong64::ULong64(const Char & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Byte & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Short & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const UShort & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Long & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const ULong & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Int & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const UInt & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Long64 & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const ULong64 & rhs)
{
	m_Value = rhs.m_Value;
}
ULong64::ULong64(const Float & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Double & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const LongDouble & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::ULong64(const Pointer & rhs)
{
	m_Value = (unsigned __int64)(rhs.getValue());
}
ULong64::~ULong64()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Float
	// constructors & destructor
Float::Float()
{
	m_Value = 0.0f;
}
Float::Float(const float & rhs)
{
	m_Value = rhs;
}
Float::Float(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1.0f : 0.0f;
}
Float::Float(const Char & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const Byte & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const Short & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const UShort & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const Long & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const ULong & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const Int & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const UInt & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const Long64 & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const ULong64 & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const Float & rhs)
{
	m_Value = rhs.m_Value;
}
Float::Float(const Double & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::Float(const LongDouble & rhs)
{
	m_Value = (float)(rhs.getValue());
}
Float::~Float()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Double
	// constructors & destructor
Double::Double()
{
	m_Value = 0.0;
}
Double::Double(const double & rhs)
{
	m_Value = rhs;
}
Double::Double(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1.0 : 0.0;
}
Double::Double(const Char & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Byte & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Short & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const UShort & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Long & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const ULong & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Int & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const UInt & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Long64 & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const ULong64 & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Float & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::Double(const Double & rhs)
{
	m_Value = rhs.m_Value;
}
Double::Double(const LongDouble & rhs)
{
	m_Value = (double)(rhs.getValue());
}
Double::~Double()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : LongDouble
	// constructors & destructor
LongDouble::LongDouble()
{
	m_Value = 0.0;
}
LongDouble::LongDouble(const long double & rhs)
{
	m_Value = rhs;
}
LongDouble::LongDouble(const Bool & rhs)
{
	m_Value = (rhs.getValue()) ? 1.0 : 0.0;
}
LongDouble::LongDouble(const Char & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Byte & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Short & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const UShort & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Long & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const ULong & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Int & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const UInt & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Long64 & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const ULong64 & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Float & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const Double & rhs)
{
	m_Value = (long double)(rhs.getValue());
}
LongDouble::LongDouble(const LongDouble & rhs)
{
	m_Value = rhs.m_Value;
}
LongDouble::~LongDouble()
{}

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Pointer
	// constructors & destructor
Pointer::Pointer()
{
	m_Value = NULL;
}
Pointer::Pointer(Object* & rhs)
{
	m_Value = rhs;
}
Pointer::Pointer(const Bool & rhs)
{
	m_Value = NULL;
}
Pointer::Pointer(const Char & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const Byte & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const Short & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const UShort & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const Long & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const ULong & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const Int & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const UInt & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const Long64 & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const ULong64 & rhs)
{
	m_Value = (Object*)(unsigned int)(rhs.getValue());
}
Pointer::Pointer(const Pointer & rhs)
{
	m_Value = rhs.m_Value;
}
Pointer::~Pointer()
{}

