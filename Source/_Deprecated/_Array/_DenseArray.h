/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/DenseArray.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Dense Arrays template interface.
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
#ifndef KOALA_LIB_CORE_DATASTRUCT_ARRAY_DENSEARRAY_H
#define KOALA_LIB_CORE_DATASTRUCT_ARRAY_DENSEARRAY_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../Datastruct.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
#define ARRAY_DENSE_PAGESIZE       256 // MUST be a power of 2

/////////////////////////////////////////////////////////////////////////////////
// The DenseArray template
template<typename T>
class _DenseArray : public Datastruct
{
public:
    // comparator : returns (l#r = +1), (r#l = -1), (l==r = 0)
    //              '#' may represent any ordering relation you wish (<,>,<=,>=,...).
    typedef Int (*Comparator)( const T & rLeft, const T & rRight );

	// constructors
	_DenseArray();
	_DenseArray( UInt iPageSize );
	_DenseArray( UInt iPageSize, const T & rItem );
	_DenseArray( UInt iPageSize, const T * arrItems, UInt iLength );
	~_DenseArray();

    // delayed construction
    virtual Void Allocate( UInt iPageSize );
    inline virtual Void Destroy();

    // operators
        // casting
    inline operator T*();
    inline operator const T*() const;
        // indexing
    inline T & operator[]( Int i );
	inline const T & operator[]( Int i ) const;
    inline T & operator[]( UInt i );
	inline const T & operator[]( UInt i ) const;
        // affectation
    inline _DenseArray<T> & operator=( const _DenseArray<T> & rhs );

    // getters
    inline virtual Bool IsEmpty() const;
	inline virtual UInt Count() const;
	inline virtual UInt SizeAllocated() const;

    // methods
    inline virtual Void Clear();

    inline Void Affect( const T * arrItems, UInt iLength );
    Void EnsureCapacity( UInt iRequiredSize, Bool bShrinkTo = false );
    Void SetShrinking( Bool bEnable, UInt iThreshold = INVALID_OFFSET );

    // searching & sorting
    UInt Search( const T & rItem, UInt iAt = 0, UInt iCount = INVALID_OFFSET ) const;
	UInt Search( Comparator pfCompare, const T & rItem, UInt iAt = 0, UInt iCount = INVALID_OFFSET ) const;
	Void QuickSort( Comparator pfCompare, UInt iAt = 0, UInt iCount = INVALID_OFFSET );

    // IO interface
        // Insert operations
    virtual Void Push( const T & rItem ) = 0;
    virtual Void Push( const T * arrItems, UInt iCount ) = 0;
    virtual Void Unshift( const T & rItem ) = 0;
    virtual Void Unshift( const T * arrItems, UInt iCount ) = 0;
    virtual Void Insert( UInt iAt, const T & rItem ) = 0;
    virtual Void Insert( UInt iAt, const T * arrItems, UInt iLength ) = 0;
        // Remove operations
    virtual Void Pop( T & outItem ) = 0;
    virtual Void Pop( T * outItems, UInt iCount ) = 0;
    virtual Void Shift( T & outItem ) = 0;
    virtual Void Shift( T * outItems, UInt iCount ) = 0;
	virtual Void Remove( UInt iAt, T & outItem ) = 0;
	virtual Void Remove( UInt iAt, T * outItems, UInt iLength ) = 0;
        // Derivates
    inline Void Push( const _DenseArray<T> & rArray, UInt iOffset = 0, UInt iLength = INVALID_OFFSET );
    inline Void Unshift( const _DenseArray<T> & rArray, UInt iOffset = 0, UInt iLength = INVALID_OFFSET );
    inline Void Insert( UInt iAt, const _DenseArray<T> & rArray, UInt iOffset = 0, UInt iLength = INVALID_OFFSET );
    inline Void Pop( _DenseArray<T> & outArray, UInt iLength );
    inline Void Shift( _DenseArray<T> & outArray, UInt iLength );
    inline Void Remove( UInt iAt, _DenseArray<T> & outArray, UInt iLength );

protected:
    // Generic Array Allocation (GAA)
        // AlignSize = aligns given size with our pagesize
    inline UInt _GAA_AlignSize( UInt iSize ) const;
        // InitNull = Initializes a NULL array with size 0
    inline Void _GAA_InitNull();
        // InitNew = Initializes a new array with given aligned-size iAlignedSize
    inline Void _GAA_InitNew( UInt iAlignedSize );
        // DeleteNull = deletes current array if non-NULL and makes an InitNull operation
    inline Void _GAA_DeleteNull();
        // DeleteNull = deletes current array if non-NULL and makes an InitNew operation
    inline Void _GAA_DeleteNew( UInt iSize );

    // Specific Array Indexing (SAI)
        // SetNull = Sets invalid indexing values for a NULL array
    virtual Void _SAI_SetNull() = 0;
        // SetNew = Sets valid initial indexing values for an iLength range
    virtual Void _SAI_SetNew( UInt iLength ) = 0;
        // Begin = Gives beginning of the range defined by the user's SAI
    virtual UInt _SAI_Begin() const = 0;
        // Length = Gives length of the range defined by the user's SAI
    virtual UInt _SAI_Length() const = 0;

    // Generalized Array Tranforms (GAT)
        // Resize = grows or shrinks allocation range by iPageDelta pages, leaves
        // content in the given range unchanged.
        // SAI values are updated by a call to _SAI_SetNew with unchanged length and
        // updated allocation size.
    Void _GAT_Resize( Int iPageDelta );
        // ShiftExtend = moves SAI range left or right in the allocation range.
        // Extend-Shifts move the bound of the shift direction's side, growing the
        // SAI range, thus creating side-holes. iSplitOffset is an offset starting
        // from the SAI range's begin and can be specified to indicate a cut index,
        // thus creating middle-holes.
        // SAI values are unchanged and must be updated accordingly.
    Void _GAT_ShiftExtend( Int iShift, UInt iSplitOffset = INVALID_OFFSET );
        // ShiftDiscard = moves SAI range left or right in the allocation range.
        // Discard-Shifts move the bound opposite to the shift direction's side,
        // shrinking the SAI range. iSplitOffset is an offset starting from the
        // SAI range's begin and can be specified to indicate a collapse index.
        // SAI values are unchanged and must be updated accordingly.
    Void _GAT_ShiftDiscard( Int iShift, UInt iSplitOffset = INVALID_OFFSET );
        // ResizeAndShift = combines a Resize and a Shift transforms "on the fly".
        // SAI values are updated by a call to _SAI_SetNew with updated length and
        // updated allocation size.
    Void _GAT_ResizeAndShiftExtend( Int iPageDelta, Int iShift, UInt iSplitOffset = INVALID_OFFSET );
    Void _GAT_ResizeAndShiftDiscard( Int iPageDelta, Int iShift, UInt iSplitOffset = INVALID_OFFSET );

    // Helpers
    inline Void _AutoGrow( UInt iSize );
    inline Void _AutoShrink();

    // Paging data
    UInt m_iPageSize;
    UInt m_iShrinkThreshold;

    // Allocation range (the array)
    UInt m_iAllocatedSize;
	T * m_arrBuffer;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "DenseArray.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_LIB_CORE_DATASTRUCT_ARRAY_DENSEARRAY_H

/////////////////////////////////////////////////////////////////////////////////
//Void _Array_StressTest()
//{
//    ULong iSeed = 131313131ul;
//    RandFn->XorShift128Seed( iSeed, 30 );
//
//    UInt iPageSize = 8;
//    //ArrayL<Int> testArray( iPageSize );
//    //ArrayR<Int> testArray( iPageSize );
//    ArrayB<Int> testArray( iPageSize );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n\nBeginning Array Tests ...") );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Index Model = Balanced") );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Page Size = %ud"), iPageSize );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Random Seed = %ud"), iSeed );
//
//    UInt iFillingOperations = 30;
//    UInt iOperations = 100;
//    UInt iValuesRange = 64;
//    const UInt iCountsRange = 4;
//    Int iValuesBuffer[iCountsRange];
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n-- Test parameters --") );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Filling = %ud operations"), iFillingOperations );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Testing = %ud operations"), iOperations );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Values range = [ 0 ; %ud ]"), iValuesRange - 1 );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Counts range = [ 1 ; %ud ]"), iCountsRange );
//
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n-- Filling Start --") );
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n => State 0 = <Empty>") );
//
//    UInt iOp;
//    UInt iTotalOps = iFillingOperations + iOperations;
//    for(UInt i = 0; i < iTotalOps; ++i) {
//        if (i < iFillingOperations)
//            iOp = ( RandFn->XorShift128() % 6 );
//        else
//            iOp = ( RandFn->XorShift128() % 12 );
//        if (i == iFillingOperations)
//            ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n-- Testing Start --") );
//        switch(iOp) {
//            case 0: { // push 1
//                    UInt iValue = ( RandFn->XorShift128() % iValuesRange );
//                    testArray.Push( iValue );
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Push_1( %ud )"), iValue );
//                } break;
//            case 1: { // push n
//                    UInt iCount = ( RandFn->XorShift128() % iCountsRange ) + 1;
//                    for(UInt j = 0; j < iCount; ++j)
//                        iValuesBuffer[j] = ( RandFn->XorShift128() % iValuesRange );
//                    testArray.Push( iValuesBuffer, iCount );
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Push_N(") );
//                    for(UInt k = 0; k < iCount; ++k) {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), iValuesBuffer[k] );
//                    }
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ) (Count = %ud)"), iCount );
//                } break;
//            case 2: { // unshift 1
//                    UInt iValue = ( RandFn->XorShift128() % iValuesRange );
//                    testArray.Unshift( iValue );
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Unshift_1( %ud )"), iValue );
//                } break;
//            case 3: { // unshift n
//                    UInt iCount = ( RandFn->XorShift128() % iCountsRange ) + 1;
//                    for(UInt j = 0; j < iCount; ++j)
//                        iValuesBuffer[j] = ( RandFn->XorShift128() % iValuesRange );
//                    testArray.Unshift( iValuesBuffer, iCount );
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Unshift_N(") );
//                    for(UInt k = 0; k < iCount; ++k) {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), iValuesBuffer[k] );
//                    }
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ) (Count = %ud)"), iCount );
//                } break;
//            case 4: { // insert 1
//                    UInt iAt = ( RandFn->XorShift128() % (testArray.Length() + 1) );
//                    UInt iValue = ( RandFn->XorShift128() % iValuesRange );
//                    testArray.Insert( iAt, iValue );
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Insert_1( (At %ud), %ud ) "), iAt, iValue );
//                } break;
//            case 5: { // insert n
//                    UInt iAt = ( RandFn->XorShift128() % (testArray.Length() + 1) );
//                    UInt iCount = ( RandFn->XorShift128() % iCountsRange ) + 1;
//                    for(UInt j = 0; j < iCount; ++j)
//                        iValuesBuffer[j] = ( RandFn->XorShift128() % iValuesRange );
//                    testArray.Insert( iAt, iValuesBuffer, iCount );
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Insert_N( (At %ud),"), iAt );
//                    for(UInt k = 0; k < iCount; ++k) {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), iValuesBuffer[k] );
//                    }
//                    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ) (Count = %ud)"), iCount );
//                } break;
//            case 6: { // pop 1
//                    Int iValue = -1;
//                    if ( testArray.Length() > 0 ) {
//                        testArray.Pop( iValue );
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Pop_1( %ud )"), iValue );
//                    } else {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = NOP (invalid Pop_1 skipped)") );
//                    }
//                } break;
//            case 7: { // pop n
//                    if ( testArray.Length() > 0 ) {
//                        UInt iCount = ( RandFn->XorShift128() % iCountsRange ) + 1;
//                        if ( iCount > testArray.Length() )
//                            iCount = testArray.Length();
//                        testArray.Pop( iValuesBuffer, iCount );
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Pop_N(") );
//                        for(UInt k = 0; k < iCount; ++k) {
//                            ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), iValuesBuffer[k] );
//                        }
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ) (Count = %ud)"), iCount );
//                    } else {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = NOP (invalid Pop_N skipped)") );
//                    }
//                } break;
//            case 8: { // shift 1
//                    Int iValue = -1;
//                    if ( testArray.Length() > 0 ) {
//                        testArray.Shift( iValue );
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Shift_1( %ud )"), iValue );
//                    } else {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = NOP (invalid Shift_1 skipped)") );
//                    }
//                } break;
//            case 9: { // shift n
//                    if ( testArray.Length() > 0 ) {
//                        UInt iCount = ( RandFn->XorShift128() % iCountsRange ) + 1;
//                        if ( iCount > testArray.Length() )
//                            iCount = testArray.Length();
//                        testArray.Shift( iValuesBuffer, iCount );
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Shift_N(") );
//                        for(UInt k = 0; k < iCount; ++k) {
//                            ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), iValuesBuffer[k] );
//                        }
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ) (Count = %ud)"), iCount );
//                    } else {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = NOP (invalid Shift_N skipped)") );
//                    }
//                } break;
//            case 10: { // remove 1
//                    Int iValue = -1;
//                    if ( testArray.Length() > 0 ) {
//                        UInt iAt = ( RandFn->XorShift128() % testArray.Length() );
//                        testArray.Remove( iAt, iValue );
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Remove_1( (At %ud), %ud )"), iAt, iValue );
//                    } else {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = NOP (invalid Remove_1 skipped)") );
//                    }
//                } break;
//            case 11: { // remove n
//                    if ( testArray.Length() > 0 ) {
//                        UInt iAt = ( RandFn->XorShift128() % testArray.Length() );
//                        UInt iCount = ( RandFn->XorShift128() % iCountsRange ) + 1;
//                        UInt iMaxCount = (testArray.Length() - iAt);
//                        if ( iCount > iMaxCount )
//                            iCount = iMaxCount;
//                        testArray.Remove( iAt, iValuesBuffer, iCount );
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = Remove_N( (At %ud),"), iAt );
//                        for(UInt k = 0; k < iCount; ++k) {
//                            ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), iValuesBuffer[k] );
//                        }
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ) (Count = %ud)"), iCount );
//                    } else {
//                        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n - Operation = NOP (invalid Remove_N skipped)") );
//                    }
//                } break;
//        }
//        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n => State %ud = ["), (i+1) );
//        for(UInt k = 0; k < testArray.Length(); ++k) {
//            ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" %ud"), testArray[k] );
//        }
//        ErrorFn->Log( TEXT("_Logs/array.log"), TEXT(" ] (Length %ud)"), testArray.Length() );
//    }
//
//    ErrorFn->Log( TEXT("_Logs/array.log"), TEXT("\n-- End of Tests --\n\n") );
//    testArray.Clear();
//}
/////////////////////////////////////////////////////////////////////////////////
