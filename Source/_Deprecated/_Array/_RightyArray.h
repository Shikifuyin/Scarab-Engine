/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/RightyArray.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Dense-Array, Righty behaviour.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_LIB_CORE_DATASTRUCT_ARRAY_RIGHTYARRAY_H
#define KOALA_LIB_CORE_DATASTRUCT_ARRAY_RIGHTYARRAY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "DenseArray.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ArrayR template
template<typename T>
class ArrayR : public _DenseArray<T>
{
public:
    // constructors
    ArrayR();
    ArrayR( UInt iPageSize );
	ArrayR( UInt iPageSize, const T & rItem );
    ArrayR( UInt iPageSize, const T * arrItems, UInt iLength );
	~ArrayR();

    inline Void Push( const T & rItem );
    inline Void Push( const T * arrItems, UInt iCount );
    inline Void Unshift( const T & rItem );
    inline Void Unshift( const T * arrItems, UInt iCount );
    inline Void Insert( UInt iAt, const T & rItem );
    inline Void Insert( UInt iAt, const T * arrItems, UInt iCount );

    inline Void Pop( T & outItem );
    inline Void Pop( T * outItems, UInt iCount );
    inline Void Shift( T & outItem );
    inline Void Shift( T * outItems, UInt iCount );
    inline Void Remove( UInt iAt, T & outItem );
	inline Void Remove( UInt iAt, T * outItems, UInt iCount );

private:
    // Specific Array Indexing (SAI)
    inline Void _SAI_SetNull();
    inline Void _SAI_SetNew( UInt iLength );
    inline UInt _SAI_Begin() const;
    inline UInt _SAI_Length() const;

    // Helpers
    inline Void _AutoGrowShift( UInt iSize, UInt iSplitOffset = INVALID_OFFSET );
    inline Void _AutoShrinkShift( UInt iSize, UInt iSplitOffset = INVALID_OFFSET );

    // Data
    UInt m_iBegin;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "RightyArray.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_LIB_CORE_DATASTRUCT_ARRAY_RIGHTYARRAY_H
