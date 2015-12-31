/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/BalancedArray.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Dense-Array, Balanced behaviour.
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
#ifndef KOALA_LIB_CORE_DATASTRUCT_ARRAY_BALANCEDARRAY_H
#define KOALA_LIB_CORE_DATASTRUCT_ARRAY_BALANCEDARRAY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "DenseArray.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The ArrayB template
template<typename T>
class ArrayB : public _DenseArray<T>
{
public:
    // constructors
    ArrayB();
    ArrayB( UInt iPageSize );
	ArrayB( UInt iPageSize, const T & rItem );
    ArrayB( UInt iPageSize, const T * arrItems, UInt iLength );
	~ArrayB();

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
    Void _RebalanceShiftGrowLeft( UInt iSize, UInt iSplitOffset = INVALID_OFFSET );
    Void _RebalanceShiftGrowRight( UInt iSize, UInt iSplitOffset = INVALID_OFFSET );
    Void _RebalanceShiftShrinkLeft( UInt iSize, UInt iSplitOffset = INVALID_OFFSET );
    Void _RebalanceShiftShrinkRight( UInt iSize, UInt iSplitOffset = INVALID_OFFSET );

    // Data
    UInt m_iBegin;
    UInt m_iNextEnd;
    UInt m_iMiddle;
    UInt m_iLeftHeavy, m_iRightHeavy;
    Int m_iBalance;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "BalancedArray.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_LIB_CORE_DATASTRUCT_ARRAY_BALANCEDARRAY_H
