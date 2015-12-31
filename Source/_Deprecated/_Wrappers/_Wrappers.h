/*******************************************************************************/
/* /object/Wrappers.hpp - Internal Header **************************************/
/*******************************************************************************/
/* KOALIB Project by Ark *******************************************************/
/*******************************************************************************/

#ifndef KOALA_LIB_OBJECT_WRAPPERS_HPP
#define KOALA_LIB_OBJECT_WRAPPERS_HPP

#include "Comparable.hpp"

// prototypes

// must be defined first
class UInt;
class Int;

class Bool;
class Char;
class Byte;
class Short;
class UShort;
class ULong;
class Long;
class Long64;
class ULong64;
class Float;
class Double;
class LongDouble;
class Pointer;


///////////////////////////////////////////////////////////////////////////////////
// Basic Types : UInt
class UInt : public Comparable
{
public:
	// constructors & destructor
	UInt();
	UInt(const unsigned int &);
	UInt(const Bool &);
	UInt(const Char &);
	UInt(const Byte &);
	UInt(const Short &);
	UInt(const UShort &);
	UInt(const Long &);
	UInt(const ULong &);
	UInt(const Int &);
	UInt(const UInt &);
	UInt(const Long64 &);
	UInt(const ULong64 &);
	UInt(const Float &);
	UInt(const Double &);
	UInt(const LongDouble &);
	UInt(const Pointer &);
	virtual ~UInt();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const UInt &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const UInt &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const UInt &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const UInt &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const UInt &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const UInt &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const UInt &)rhs).m_Value ); }
	
	// operators
	inline virtual UInt operator-() const
		{ return UInt( -m_Value ); }
	inline virtual UInt operator+(const UInt & rhs) const
		{ return UInt(m_Value + rhs.m_Value); }
	inline virtual UInt operator-(const UInt & rhs) const
		{ return UInt(m_Value - rhs.m_Value); }
	inline virtual UInt operator*(const UInt & rhs) const
		{ return UInt(m_Value * rhs.m_Value); }
	inline virtual UInt operator/(const UInt & rhs) const
		{ return UInt(m_Value / rhs.m_Value); }
	inline virtual UInt operator%(const UInt & rhs) const
		{ return UInt(m_Value % rhs.m_Value); }
	
	inline virtual UInt operator~() const
		{ return UInt( ~m_Value ); }
	inline virtual UInt operator<<(const UInt & rhs) const
		{ return UInt(m_Value << rhs.getValue()); }
	inline virtual UInt operator>>(const UInt & rhs) const
		{ return UInt(m_Value >> rhs.getValue()); }
	inline virtual UInt operator&(const UInt & rhs) const
		{ return UInt(m_Value & rhs.m_Value); }
	inline virtual UInt operator|(const UInt & rhs) const
		{ return UInt(m_Value | rhs.m_Value); }
	inline virtual UInt operator^(const UInt & rhs) const
		{ return UInt(m_Value ^ rhs.m_Value); }

	inline virtual UInt & operator+=(const UInt & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual UInt & operator-=(const UInt & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual UInt & operator*=(const UInt & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual UInt & operator/=(const UInt & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual UInt & operator%=(const UInt & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual UInt & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual UInt & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual UInt & operator&=(const UInt & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual UInt & operator|=(const UInt & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual UInt & operator^=(const UInt & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline unsigned int getValue() const { return m_Value; }

private:
	unsigned int m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Int
class Int : public Comparable
{
public:
	// constructors & destructor
	Int();
	Int(const signed int &);
	Int(const Bool &);
	Int(const Char &);
	Int(const Byte &);
	Int(const Short &);
	Int(const UShort &);
	Int(const Long &);
	Int(const ULong &);
	Int(const Int &);
	Int(const UInt &);
	Int(const Long64 &);
	Int(const ULong64 &);
	Int(const Float &);
	Int(const Double &);
	Int(const LongDouble &);
	Int(const Pointer &);
	virtual ~Int();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Int &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Int &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Int &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Int &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Int &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Int &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Int &)rhs).m_Value ); }
	
	// operators
	inline virtual Int operator-() const
		{ return Int( -m_Value ); }
	inline virtual Int operator+(const Int & rhs) const
		{ return Int(m_Value + rhs.m_Value); }
	inline virtual Int operator-(const Int & rhs) const
		{ return Int(m_Value - rhs.m_Value); }
	inline virtual Int operator*(const Int & rhs) const
		{ return Int(m_Value * rhs.m_Value); }
	inline virtual Int operator/(const Int & rhs) const
		{ return Int(m_Value / rhs.m_Value); }
	inline virtual Int operator%(const Int & rhs) const
		{ return Int(m_Value % rhs.m_Value); }
	
	inline virtual Int operator~() const
		{ return Int( ~m_Value ); }
	inline virtual Int operator<<(const UInt & rhs) const
		{ return Int(m_Value << rhs.getValue()); }
	inline virtual Int operator>>(const UInt & rhs) const
		{ return Int(m_Value >> rhs.getValue()); }
	inline virtual Int operator&(const Int & rhs) const
		{ return Int(m_Value & rhs.m_Value); }
	inline virtual Int operator|(const Int & rhs) const
		{ return Int(m_Value | rhs.m_Value); }
	inline virtual Int operator^(const Int & rhs) const
		{ return Int(m_Value ^ rhs.m_Value); }

	inline virtual Int & operator+=(const Int & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Int & operator-=(const Int & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Int & operator*=(const Int & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Int & operator/=(const Int & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual Int & operator%=(const Int & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual Int & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual Int & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual Int & operator&=(const Int & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual Int & operator|=(const Int & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual Int & operator^=(const Int & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline signed int getValue() const { return m_Value; }

private:
	signed int m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Bool
class Bool : public Object
{
public:
	// constructors & destructor
	Bool();
	Bool(const bool &);
	Bool(const Bool &);
	Bool(const Char &);
	Bool(const Byte &);
	Bool(const Short &);
	Bool(const UShort &);
	Bool(const Long &);
	Bool(const ULong &);
	Bool(const Int &);
	Bool(const UInt &);
	Bool(const Long64 &);
	Bool(const ULong64 &);
	Bool(const Float &);
	Bool(const Double &);
	Bool(const LongDouble &);
	Bool(const Pointer &);
	virtual ~Bool();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Bool &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Bool &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Bool &)rhs).m_Value); }

	// operators
	inline virtual Bool operator&&(const Bool & rhs) const { return Bool(m_Value && rhs.m_Value); }
	inline virtual Bool operator||(const Bool & rhs) const { return Bool(m_Value || rhs.m_Value); }
	inline virtual Bool operator!() const { return Bool(!m_Value); }

	// getters & setters
	inline bool getValue() const { return m_Value; }

private:
	bool m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Char
class Char : public Comparable
{
public:
	// constructors & destructor
	Char();
	Char(const signed char &);
	Char(const Bool &);
	Char(const Char &);
	Char(const Byte &);
	Char(const Short &);
	Char(const UShort &);
	Char(const Long &);
	Char(const ULong &);
	Char(const Int &);
	Char(const UInt &);
	Char(const Long64 &);
	Char(const ULong64 &);
	Char(const Float &);
	Char(const Double &);
	Char(const LongDouble &);
	Char(const Pointer &);
	virtual ~Char();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Char &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Char &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Char &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Char &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Char &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Char &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Char &)rhs).m_Value ); }
	
	// operators
	inline virtual Char operator-() const
		{ return Char( -m_Value ); }
	inline virtual Char operator+(const Char & rhs) const
		{ return Char(m_Value + rhs.m_Value); }
	inline virtual Char operator-(const Char & rhs) const
		{ return Char(m_Value - rhs.m_Value); }
	inline virtual Char operator*(const Char & rhs) const
		{ return Char(m_Value * rhs.m_Value); }
	inline virtual Char operator/(const Char & rhs) const
		{ return Char(m_Value / rhs.m_Value); }
	inline virtual Char operator%(const Char & rhs) const
		{ return Char(m_Value % rhs.m_Value); }
	
	inline virtual Char operator~() const
		{ return Char( ~m_Value ); }
	inline virtual Char operator<<(const UInt & rhs) const
		{ return Char(m_Value << rhs.getValue()); }
	inline virtual Char operator>>(const UInt & rhs) const
		{ return Char(m_Value >> rhs.getValue()); }
	inline virtual Char operator&(const Char & rhs) const
		{ return Char(m_Value & rhs.m_Value); }
	inline virtual Char operator|(const Char & rhs) const
		{ return Char(m_Value | rhs.m_Value); }
	inline virtual Char operator^(const Char & rhs) const
		{ return Char(m_Value ^ rhs.m_Value); }

	inline virtual Char & operator+=(const Char & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Char & operator-=(const Char & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Char & operator*=(const Char & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Char & operator/=(const Char & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual Char & operator%=(const Char & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual Char & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual Char & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual Char & operator&=(const Char & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual Char & operator|=(const Char & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual Char & operator^=(const Char & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline signed char getValue() const { return m_Value; }

private:
	signed char m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Byte
class Byte : public Comparable
{
public:
	// constructors & destructor
	Byte();
	Byte(const unsigned char &);
	Byte(const Bool &);
	Byte(const Char &);
	Byte(const Byte &);
	Byte(const Short &);
	Byte(const UShort &);
	Byte(const Long &);
	Byte(const ULong &);
	Byte(const Int &);
	Byte(const UInt &);
	Byte(const Long64 &);
	Byte(const ULong64 &);
	Byte(const Float &);
	Byte(const Double &);
	Byte(const LongDouble &);
	Byte(const Pointer &);
	virtual ~Byte();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Byte &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Byte &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Byte &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Byte &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Byte &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Byte &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Byte &)rhs).m_Value ); }
	
	// operators
	inline virtual Byte operator-() const
		{ return Byte( -m_Value ); }
	inline virtual Byte operator+(const Byte & rhs) const
		{ return Byte(m_Value + rhs.m_Value); }
	inline virtual Byte operator-(const Byte & rhs) const
		{ return Byte(m_Value - rhs.m_Value); }
	inline virtual Byte operator*(const Byte & rhs) const
		{ return Byte(m_Value * rhs.m_Value); }
	inline virtual Byte operator/(const Byte & rhs) const
		{ return Byte(m_Value / rhs.m_Value); }
	inline virtual Byte operator%(const Byte & rhs) const
		{ return Byte(m_Value % rhs.m_Value); }
	
	inline virtual Byte operator~() const
		{ return Byte( ~m_Value ); }
	inline virtual Byte operator<<(const UInt & rhs) const
		{ return Byte(m_Value << rhs.getValue()); }
	inline virtual Byte operator>>(const UInt & rhs) const
		{ return Byte(m_Value >> rhs.getValue()); }
	inline virtual Byte operator&(const Byte & rhs) const
		{ return Byte(m_Value & rhs.m_Value); }
	inline virtual Byte operator|(const Byte & rhs) const
		{ return Byte(m_Value | rhs.m_Value); }
	inline virtual Byte operator^(const Byte & rhs) const
		{ return Byte(m_Value ^ rhs.m_Value); }

	inline virtual Byte & operator+=(const Byte & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Byte & operator-=(const Byte & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Byte & operator*=(const Byte & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Byte & operator/=(const Byte & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual Byte & operator%=(const Byte & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual Byte & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual Byte & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual Byte & operator&=(const Byte & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual Byte & operator|=(const Byte & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual Byte & operator^=(const Byte & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline unsigned char getValue() const { return m_Value; }

private:
	unsigned char m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Short
class Short : public Comparable
{
public:
	// constructors & destructor
	Short();
	Short(const signed short &);
	Short(const Bool &);
	Short(const Char &);
	Short(const Byte &);
	Short(const Short &);
	Short(const UShort &);
	Short(const Long &);
	Short(const ULong &);
	Short(const Int &);
	Short(const UInt &);
	Short(const Long64 &);
	Short(const ULong64 &);
	Short(const Float &);
	Short(const Double &);
	Short(const LongDouble &);
	Short(const Pointer &);
	virtual ~Short();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Short &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Short &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Short &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Short &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Short &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Short &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Short &)rhs).m_Value ); }
	
	// operators
	inline virtual Short operator-() const
		{ return Short( -m_Value ); }
	inline virtual Short operator+(const Short & rhs) const
		{ return Short(m_Value + rhs.m_Value); }
	inline virtual Short operator-(const Short & rhs) const
		{ return Short(m_Value - rhs.m_Value); }
	inline virtual Short operator*(const Short & rhs) const
		{ return Short(m_Value * rhs.m_Value); }
	inline virtual Short operator/(const Short & rhs) const
		{ return Short(m_Value / rhs.m_Value); }
	inline virtual Short operator%(const Short & rhs) const
		{ return Short(m_Value % rhs.m_Value); }
	
	inline virtual Short operator~() const
		{ return Short( ~m_Value ); }
	inline virtual Short operator<<(const UInt & rhs) const
		{ return Short(m_Value << rhs.getValue()); }
	inline virtual Short operator>>(const UInt & rhs) const
		{ return Short(m_Value >> rhs.getValue()); }
	inline virtual Short operator&(const Short & rhs) const
		{ return Short(m_Value & rhs.m_Value); }
	inline virtual Short operator|(const Short & rhs) const
		{ return Short(m_Value | rhs.m_Value); }
	inline virtual Short operator^(const Short & rhs) const
		{ return Short(m_Value ^ rhs.m_Value); }

	inline virtual Short & operator+=(const Short & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Short & operator-=(const Short & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Short & operator*=(const Short & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Short & operator/=(const Short & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual Short & operator%=(const Short & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual Short & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual Short & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual Short & operator&=(const Short & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual Short & operator|=(const Short & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual Short & operator^=(const Short & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline signed short getValue() const { return m_Value; }

private:
	signed short m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : UShort
class UShort : public Comparable
{
public:
	// constructors & destructor
	UShort();
	UShort(const unsigned short &);
	UShort(const Bool &);
	UShort(const Char &);
	UShort(const Byte &);
	UShort(const Short &);
	UShort(const UShort &);
	UShort(const Long &);
	UShort(const ULong &);
	UShort(const Int &);
	UShort(const UInt &);
	UShort(const Long64 &);
	UShort(const ULong64 &);
	UShort(const Float &);
	UShort(const Double &);
	UShort(const LongDouble &);
	UShort(const Pointer &);
	virtual ~UShort();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const UShort &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const UShort &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const UShort &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const UShort &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const UShort &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const UShort &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const UShort &)rhs).m_Value ); }
	
	// operators
	inline virtual UShort operator-() const
		{ return UShort( -m_Value ); }
	inline virtual UShort operator+(const UShort & rhs) const
		{ return UShort(m_Value + rhs.m_Value); }
	inline virtual UShort operator-(const UShort & rhs) const
		{ return UShort(m_Value - rhs.m_Value); }
	inline virtual UShort operator*(const UShort & rhs) const
		{ return UShort(m_Value * rhs.m_Value); }
	inline virtual UShort operator/(const UShort & rhs) const
		{ return UShort(m_Value / rhs.m_Value); }
	inline virtual UShort operator%(const UShort & rhs) const
		{ return UShort(m_Value % rhs.m_Value); }
	
	inline virtual UShort operator~() const
		{ return UShort( ~m_Value ); }
	inline virtual UShort operator<<(const UInt & rhs) const
		{ return UShort(m_Value << rhs.getValue()); }
	inline virtual UShort operator>>(const UInt & rhs) const
		{ return UShort(m_Value >> rhs.getValue()); }
	inline virtual UShort operator&(const UShort & rhs) const
		{ return UShort(m_Value & rhs.m_Value); }
	inline virtual UShort operator|(const UShort & rhs) const
		{ return UShort(m_Value | rhs.m_Value); }
	inline virtual UShort operator^(const UShort & rhs) const
		{ return UShort(m_Value ^ rhs.m_Value); }

	inline virtual UShort & operator+=(const UShort & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual UShort & operator-=(const UShort & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual UShort & operator*=(const UShort & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual UShort & operator/=(const UShort & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual UShort & operator%=(const UShort & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual UShort & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual UShort & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual UShort & operator&=(const UShort & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual UShort & operator|=(const UShort & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual UShort & operator^=(const UShort & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline unsigned short getValue() const { return m_Value; }

private:
	unsigned short m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Long
class Long : public Comparable
{
public:
	// constructors & destructor
	Long();
	Long(const signed long &);
	Long(const Bool &);
	Long(const Char &);
	Long(const Byte &);
	Long(const Short &);
	Long(const UShort &);
	Long(const Long &);
	Long(const ULong &);
	Long(const Int &);
	Long(const UInt &);
	Long(const Long64 &);
	Long(const ULong64 &);
	Long(const Float &);
	Long(const Double &);
	Long(const LongDouble &);
	Long(const Pointer &);
	virtual ~Long();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Long &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Long &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Long &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Long &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Long &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Long &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Long &)rhs).m_Value ); }
	
	// operators
	inline virtual Long operator-() const
		{ return Long( -m_Value ); }
	inline virtual Long operator+(const Long & rhs) const
		{ return Long(m_Value + rhs.m_Value); }
	inline virtual Long operator-(const Long & rhs) const
		{ return Long(m_Value - rhs.m_Value); }
	inline virtual Long operator*(const Long & rhs) const
		{ return Long(m_Value * rhs.m_Value); }
	inline virtual Long operator/(const Long & rhs) const
		{ return Long(m_Value / rhs.m_Value); }
	inline virtual Long operator%(const Long & rhs) const
		{ return Long(m_Value % rhs.m_Value); }
	
	inline virtual Long operator~() const
		{ return Long( ~m_Value ); }
	inline virtual Long operator<<(const UInt & rhs) const
		{ return Long(m_Value << rhs.getValue()); }
	inline virtual Long operator>>(const UInt & rhs) const
		{ return Long(m_Value >> rhs.getValue()); }
	inline virtual Long operator&(const Long & rhs) const
		{ return Long(m_Value & rhs.m_Value); }
	inline virtual Long operator|(const Long & rhs) const
		{ return Long(m_Value | rhs.m_Value); }
	inline virtual Long operator^(const Long & rhs) const
		{ return Long(m_Value ^ rhs.m_Value); }

	inline virtual Long & operator+=(const Long & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Long & operator-=(const Long & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Long & operator*=(const Long & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Long & operator/=(const Long & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual Long & operator%=(const Long & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual Long & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual Long & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual Long & operator&=(const Long & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual Long & operator|=(const Long & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual Long & operator^=(const Long & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline signed long getValue() const { return m_Value; }

private:
	signed long m_Value;
};


///////////////////////////////////////////////////////////////////////////////////
// Basic Types : ULong
class ULong : public Comparable
{
public:
	// constructors & destructor
	ULong();
	ULong(const unsigned long &);
	ULong(const Bool &);
	ULong(const Char &);
	ULong(const Byte &);
	ULong(const Short &);
	ULong(const UShort &);
	ULong(const Long &);
	ULong(const ULong &);
	ULong(const Int &);
	ULong(const UInt &);
	ULong(const Long64 &);
	ULong(const ULong64 &);
	ULong(const Float &);
	ULong(const Double &);
	ULong(const LongDouble &);
	ULong(const Pointer &);
	virtual ~ULong();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const ULong &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const ULong &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const ULong &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const ULong &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const ULong &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const ULong &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const ULong &)rhs).m_Value ); }
	
	// operators
	inline virtual ULong operator-() const
		{ return ULong( -m_Value ); }
	inline virtual ULong operator+(const ULong & rhs) const
		{ return ULong(m_Value + rhs.m_Value); }
	inline virtual ULong operator-(const ULong & rhs) const
		{ return ULong(m_Value - rhs.m_Value); }
	inline virtual ULong operator*(const ULong & rhs) const
		{ return ULong(m_Value * rhs.m_Value); }
	inline virtual ULong operator/(const ULong & rhs) const
		{ return ULong(m_Value / rhs.m_Value); }
	inline virtual ULong operator%(const ULong & rhs) const
		{ return ULong(m_Value % rhs.m_Value); }
	
	inline virtual ULong operator~() const
		{ return ULong( ~m_Value ); }
	inline virtual ULong operator<<(const UInt & rhs) const
		{ return ULong(m_Value << rhs.getValue()); }
	inline virtual ULong operator>>(const UInt & rhs) const
		{ return ULong(m_Value >> rhs.getValue()); }
	inline virtual ULong operator&(const ULong & rhs) const
		{ return ULong(m_Value & rhs.m_Value); }
	inline virtual ULong operator|(const ULong & rhs) const
		{ return ULong(m_Value | rhs.m_Value); }
	inline virtual ULong operator^(const ULong & rhs) const
		{ return ULong(m_Value ^ rhs.m_Value); }

	inline virtual ULong & operator+=(const ULong & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual ULong & operator-=(const ULong & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual ULong & operator*=(const ULong & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual ULong & operator/=(const ULong & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual ULong & operator%=(const ULong & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual ULong & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual ULong & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual ULong & operator&=(const ULong & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual ULong & operator|=(const ULong & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual ULong & operator^=(const ULong & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline unsigned long getValue() const { return m_Value; }

private:
	unsigned long m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Long64
class Long64 : public Comparable
{
public:
	// constructors & destructor
	Long64();
	Long64(const signed __int64 &);
	Long64(const Bool &);
	Long64(const Char &);
	Long64(const Byte &);
	Long64(const Short &);
	Long64(const UShort &);
	Long64(const Long &);
	Long64(const ULong &);
	Long64(const Int &);
	Long64(const UInt &);
	Long64(const Long64 &);
	Long64(const ULong64 &);
	Long64(const Float &);
	Long64(const Double &);
	Long64(const LongDouble &);
	Long64(const Pointer &);
	virtual ~Long64();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Long64 &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Long64 &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Long64 &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Long64 &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Long64 &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Long64 &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Long64 &)rhs).m_Value ); }
	
	// operators
	inline virtual Long64 operator-() const
		{ return Long64( -m_Value ); }
	inline virtual Long64 operator+(const Long64 & rhs) const
		{ return Long64(m_Value + rhs.m_Value); }
	inline virtual Long64 operator-(const Long64 & rhs) const
		{ return Long64(m_Value - rhs.m_Value); }
	inline virtual Long64 operator*(const Long64 & rhs) const
		{ return Long64(m_Value * rhs.m_Value); }
	inline virtual Long64 operator/(const Long64 & rhs) const
		{ return Long64(m_Value / rhs.m_Value); }
	inline virtual Long64 operator%(const Long64 & rhs) const
		{ return Long64(m_Value % rhs.m_Value); }
	
	inline virtual Long64 operator~() const
		{ return Long64( ~m_Value ); }
	inline virtual Long64 operator<<(const UInt & rhs) const
		{ return Long64(m_Value << rhs.getValue()); }
	inline virtual Long64 operator>>(const UInt & rhs) const
		{ return Long64(m_Value >> rhs.getValue()); }
	inline virtual Long64 operator&(const Long64 & rhs) const
		{ return Long64(m_Value & rhs.m_Value); }
	inline virtual Long64 operator|(const Long64 & rhs) const
		{ return Long64(m_Value | rhs.m_Value); }
	inline virtual Long64 operator^(const Long64 & rhs) const
		{ return Long64(m_Value ^ rhs.m_Value); }

	inline virtual Long64 & operator+=(const Long64 & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Long64 & operator-=(const Long64 & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Long64 & operator*=(const Long64 & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Long64 & operator/=(const Long64 & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual Long64 & operator%=(const Long64 & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual Long64 & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual Long64 & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual Long64 & operator&=(const Long64 & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual Long64 & operator|=(const Long64 & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual Long64 & operator^=(const Long64 & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline signed __int64 getValue() const { return m_Value; }

private:
	signed __int64 m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : ULong64
class ULong64 : public Comparable
{
public:
	// constructors & destructor
	ULong64();
	ULong64(const unsigned __int64 &);
	ULong64(const Bool &);
	ULong64(const Char &);
	ULong64(const Byte &);
	ULong64(const Short &);
	ULong64(const UShort &);
	ULong64(const Long &);
	ULong64(const ULong &);
	ULong64(const Int &);
	ULong64(const UInt &);
	ULong64(const Long64 &);
	ULong64(const ULong64 &);
	ULong64(const Float &);
	ULong64(const Double &);
	ULong64(const LongDouble &);
	ULong64(const Pointer &);
	virtual ~ULong64();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const ULong64 &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const ULong64 &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const ULong64 &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const ULong64 &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const ULong64 &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const ULong64 &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const ULong64 &)rhs).m_Value ); }
	
	// operators
	inline virtual ULong64 operator-() const
		{ return ULong64( -m_Value ); }
	inline virtual ULong64 operator+(const ULong64 & rhs) const
		{ return ULong64(m_Value + rhs.m_Value); }
	inline virtual ULong64 operator-(const ULong64 & rhs) const
		{ return ULong64(m_Value - rhs.m_Value); }
	inline virtual ULong64 operator*(const ULong64 & rhs) const
		{ return ULong64(m_Value * rhs.m_Value); }
	inline virtual ULong64 operator/(const ULong64 & rhs) const
		{ return ULong64(m_Value / rhs.m_Value); }
	inline virtual ULong64 operator%(const ULong64 & rhs) const
		{ return ULong64(m_Value % rhs.m_Value); }
	
	inline virtual ULong64 operator~() const
		{ return ULong64( ~m_Value ); }
	inline virtual ULong64 operator<<(const UInt & rhs) const
		{ return ULong64(m_Value << rhs.getValue()); }
	inline virtual ULong64 operator>>(const UInt & rhs) const
		{ return ULong64(m_Value >> rhs.getValue()); }
	inline virtual ULong64 operator&(const ULong64 & rhs) const
		{ return ULong64(m_Value & rhs.m_Value); }
	inline virtual ULong64 operator|(const ULong64 & rhs) const
		{ return ULong64(m_Value | rhs.m_Value); }
	inline virtual ULong64 operator^(const ULong64 & rhs) const
		{ return ULong64(m_Value ^ rhs.m_Value); }

	inline virtual ULong64 & operator+=(const ULong64 & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual ULong64 & operator-=(const ULong64 & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual ULong64 & operator*=(const ULong64 & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual ULong64 & operator/=(const ULong64 & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }
	inline virtual ULong64 & operator%=(const ULong64 & rhs)
		{ m_Value %= rhs.m_Value; return (*this); }

	inline virtual ULong64 & operator<<=(const UInt & rhs)
		{ m_Value <<= rhs.getValue(); return (*this); }
	inline virtual ULong64 & operator>>=(const UInt & rhs)
		{ m_Value >>= rhs.getValue(); return (*this); }
	inline virtual ULong64 & operator&=(const ULong64 & rhs)
		{ m_Value &= rhs.m_Value; return (*this); }
	inline virtual ULong64 & operator|=(const ULong64 & rhs)
		{ m_Value |= rhs.m_Value; return (*this); }
	inline virtual ULong64 & operator^=(const ULong64 & rhs)
		{ m_Value ^= rhs.m_Value; return (*this); }

	// getters & setters
	inline unsigned __int64 getValue() const { return m_Value; }

private:
	unsigned __int64 m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Float
class Float : public Comparable
{
public:
	// constructors & destructor
	Float();
	Float(const float &);
	Float(const Bool &);
	Float(const Char &);
	Float(const Byte &);
	Float(const Short &);
	Float(const UShort &);
	Float(const Long &);
	Float(const ULong &);
	Float(const Int &);
	Float(const UInt &);
	Float(const Long64 &);
	Float(const ULong64 &);
	Float(const Float &);
	Float(const Double &);
	Float(const LongDouble &);
	virtual ~Float();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Float &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Float &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Float &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Float &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Float &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Float &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Float &)rhs).m_Value ); }
	
	// operators
	inline virtual Float operator-() const
		{ return Float( -m_Value ); }
	inline virtual Float operator+(const Float & rhs) const
		{ return Float(m_Value + rhs.m_Value); }
	inline virtual Float operator-(const Float & rhs) const
		{ return Float(m_Value - rhs.m_Value); }
	inline virtual Float operator*(const Float & rhs) const
		{ return Float(m_Value * rhs.m_Value); }
	inline virtual Float operator/(const Float & rhs) const
		{ return Float(m_Value / rhs.m_Value); }

	inline virtual Float & operator+=(const Float & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Float & operator-=(const Float & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Float & operator*=(const Float & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Float & operator/=(const Float & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }

	// getters & setters
	inline float getValue() const { return m_Value; }

private:
	float m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Double
class Double : public Comparable
{
public:
	// constructors & destructor
	Double();
	Double(const double &);
	Double(const Bool &);
	Double(const Char &);
	Double(const Byte &);
	Double(const Short &);
	Double(const UShort &);
	Double(const Long &);
	Double(const ULong &);
	Double(const Int &);
	Double(const UInt &);
	Double(const Long64 &);
	Double(const ULong64 &);
	Double(const Float &);
	Double(const Double &);
	Double(const LongDouble &);
	virtual ~Double();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Double &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Double &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Double &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Double &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Double &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Double &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Double &)rhs).m_Value ); }
	
	// operators
	inline virtual Double operator-() const
		{ return Double( -m_Value ); }
	inline virtual Double operator+(const Double & rhs) const
		{ return Double(m_Value + rhs.m_Value); }
	inline virtual Double operator-(const Double & rhs) const
		{ return Double(m_Value - rhs.m_Value); }
	inline virtual Double operator*(const Double & rhs) const
		{ return Double(m_Value * rhs.m_Value); }
	inline virtual Double operator/(const Double & rhs) const
		{ return Double(m_Value / rhs.m_Value); }

	inline virtual Double & operator+=(const Double & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual Double & operator-=(const Double & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual Double & operator*=(const Double & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual Double & operator/=(const Double & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }

	// getters & setters
	inline double getValue() const { return m_Value; }

private:
	double m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : LongDouble
class LongDouble : public Comparable
{
public:
	// constructors & destructor
	LongDouble();
	LongDouble(const long double &);
	LongDouble(const Bool &);
	LongDouble(const Char &);
	LongDouble(const Byte &);
	LongDouble(const Short &);
	LongDouble(const UShort &);
	LongDouble(const Long &);
	LongDouble(const ULong &);
	LongDouble(const Int &);
	LongDouble(const UInt &);
	LongDouble(const Long64 &);
	LongDouble(const ULong64 &);
	LongDouble(const Float &);
	LongDouble(const Double &);
	LongDouble(const LongDouble &);
	virtual ~LongDouble();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const LongDouble &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const LongDouble &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const LongDouble &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const LongDouble &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const LongDouble &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const LongDouble &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const LongDouble &)rhs).m_Value ); }
	
	// operators
	inline virtual LongDouble operator-() const
		{ return LongDouble( -m_Value ); }
	inline virtual LongDouble operator+(const LongDouble & rhs) const
		{ return LongDouble(m_Value + rhs.m_Value); }
	inline virtual LongDouble operator-(const LongDouble & rhs) const
		{ return LongDouble(m_Value - rhs.m_Value); }
	inline virtual LongDouble operator*(const LongDouble & rhs) const
		{ return LongDouble(m_Value * rhs.m_Value); }
	inline virtual LongDouble operator/(const LongDouble & rhs) const
		{ return LongDouble(m_Value / rhs.m_Value); }

	inline virtual LongDouble & operator+=(const LongDouble & rhs)
		{ m_Value += rhs.m_Value; return (*this); }
	inline virtual LongDouble & operator-=(const LongDouble & rhs)
		{ m_Value -= rhs.m_Value; return (*this); }
	inline virtual LongDouble & operator*=(const LongDouble & rhs)
		{ m_Value *= rhs.m_Value; return (*this); }
	inline virtual LongDouble & operator/=(const LongDouble & rhs)
		{ m_Value /= rhs.m_Value; return (*this); }

	// getters & setters
	inline long double getValue() const { return m_Value; }

private:
	long double m_Value;
};

///////////////////////////////////////////////////////////////////////////////////
// Basic Types : Pointer
class Pointer : public Comparable
{
public:
	// constructors & destructor
	Pointer();
	Pointer(Object* &);
	Pointer(const Bool &);
	Pointer(const Char &);
	Pointer(const Byte &);
	Pointer(const Short &);
	Pointer(const UShort &);
	Pointer(const Long &);
	Pointer(const ULong &);
	Pointer(const Int &);
	Pointer(const UInt &);
	Pointer(const Long64 &);
	Pointer(const ULong64 &);
	Pointer(const Pointer &);
	virtual ~Pointer();

	// overloads from Object
	inline virtual Object & operator=(const Object & rhs)
		{ m_Value = ((const Pointer &)rhs).m_Value; return (*this); }
	inline virtual bool operator==(const Object & rhs) const
		{ return (m_Value == ((const Pointer &)rhs).m_Value); }
	inline virtual bool operator!=(const Object & rhs) const
		{ return (m_Value != ((const Pointer &)rhs).m_Value); }

	// overloads from Comparable
	inline virtual bool operator<(const Comparable & rhs) const
		{ return ( m_Value < ((const Pointer &)rhs).m_Value ); }
	inline virtual bool operator<=(const Comparable & rhs) const
		{ return ( m_Value <= ((const Pointer &)rhs).m_Value ); }
	inline virtual bool operator>(const Comparable & rhs) const
		{ return ( m_Value > ((const Pointer &)rhs).m_Value ); }
	inline virtual bool operator>=(const Comparable & rhs) const
		{ return ( m_Value >= ((const Pointer &)rhs).m_Value ); }
	
	// operators
	inline virtual Pointer operator+(const UInt & rhs) const
		{ return Pointer(m_Value + rhs.getValue()); }
	inline virtual Pointer operator-(const UInt & rhs) const
		{ return Pointer(m_Value - rhs.getValue()); }

	inline virtual Pointer & operator+=(const UInt & rhs)
		{ m_Value += rhs.getValue(); return (*this); }
	inline virtual Pointer & operator-=(const UInt & rhs)
		{ m_Value -= rhs.getValue(); return (*this); }

	// getters & setters
	inline Object * getValue() const { return m_Value; }

private:
	Object * m_Value;
};

#endif // EOF
