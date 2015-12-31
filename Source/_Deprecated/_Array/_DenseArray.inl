/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/DenseArray.inl
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
// _DenseArray Implementation
template<typename T>
_DenseArray<T>::_DenseArray():
    Datastruct()
{
    m_iPageSize = 0;
    m_iShrinkThreshold = INVALID_OFFSET;

    _GAA_InitNull();
}
template<typename T>
_DenseArray<T>::_DenseArray( UInt iPageSize ):
    Datastruct()
{
    m_iPageSize = iPageSize;
    m_iShrinkThreshold = m_iPageSize;
    m_iShrinkThreshold += (m_iPageSize >> 1);
    m_iShrinkThreshold += (m_iPageSize >> 2);

    _GAA_InitNew( _GAA_AlignSize(0) );
}
template<typename T>
_DenseArray<T>::_DenseArray( UInt iPageSize, const T & rItem ):
    Datastruct()
{
    m_iPageSize = iPageSize;
    m_iShrinkThreshold = m_iPageSize;
    m_iShrinkThreshold += (m_iPageSize >> 1);
    m_iShrinkThreshold += (m_iPageSize >> 2);

    _GAA_InitNew( _GAA_AlignSize(1) );
}
template<typename T>
_DenseArray<T>::_DenseArray( UInt iPageSize, const T * arrItems, UInt iLength ):
    Datastruct()
{
    m_iPageSize = iPageSize;
    m_iShrinkThreshold = m_iPageSize;
    m_iShrinkThreshold += (m_iPageSize >> 1);
    m_iShrinkThreshold += (m_iPageSize >> 2);

    _GAA_InitNew( _GAA_AlignSize(iLength) );
}
template<typename T>
_DenseArray<T>::~_DenseArray()
{
    // nothing to do
    // Call Destroy explicitly
}

template<typename T>
Void _DenseArray<T>::Allocate( UInt iPageSize )
{
    Assert( m_arrBuffer == NULL );

    m_iPageSize = iPageSize;
    m_iShrinkThreshold = m_iPageSize;
    m_iShrinkThreshold += (m_iPageSize >> 1);
    m_iShrinkThreshold += (m_iPageSize >> 2);

    _GAA_InitNew( _GAA_AlignSize(0) );
    _SAI_SetNew( 0 );
}
template<typename T>
inline Void _DenseArray<T>::Destroy() {
    _GAA_DeleteNull();
    _SAI_SetNull();

    m_iPageSize = 0;
    m_iShrinkThreshold = INVALID_OFFSET;
}

template<typename T>
inline _DenseArray<T>::operator T*() {
    Assert( m_arrBuffer != NULL );
    return ( m_arrBuffer + _SAI_Begin() );
}
template<typename T>
inline _DenseArray<T>::operator const T*() const {
    Assert( m_arrBuffer != NULL );
    return ( m_arrBuffer + _SAI_Begin() );
}

template<typename T>
inline T & _DenseArray<T>::operator[]( Int i ) {
    Assert( m_arrBuffer != NULL );
    Assert( (i >= 0) && ((UInt)i < _SAI_Length()) );
    return *( m_arrBuffer + _SAI_Begin() + i );
}
template<typename T>
inline const T & _DenseArray<T>::operator[]( Int i ) const {
    Assert( m_arrBuffer != NULL );
    Assert( (i >= 0) && ((UInt)i < _SAI_Length()) );
    return *( m_arrBuffer + _SAI_Begin() + i );
}
template<typename T>
inline T & _DenseArray<T>::operator[]( UInt i ) {
    Assert( m_arrBuffer != NULL );
    Assert( i < _SAI_Length() );
    return *( m_arrBuffer + _SAI_Begin() + i );
}
template<typename T>
inline const T & _DenseArray<T>::operator[]( UInt i ) const {
    Assert( m_arrBuffer != NULL );
    Assert( i < _SAI_Length() );
    return *( m_arrBuffer + _SAI_Begin() + i );
}

template<typename T>
inline _DenseArray<T> & _DenseArray<T>::operator=( const _DenseArray<T> & rhs ) {
    if ( this == &rhs )
        return (*this);
    UInt iLength = rhs.Length();
    if ( rhs.m_arrBuffer == NULL )
        Destroy();
    else if ( iLength == 0 )
        Clear();
    else
        Affect( rhs.m_arrBuffer + rhs._SAI_Begin(), iLength );
    return (*this);
}

template<typename T>
inline Bool _DenseArray<T>::IsEmpty() const {
    Assert( m_arrBuffer != NULL );
    return (_SAI_Length() == 0);
}
template<typename T>
inline UInt _DenseArray<T>::Count() const {
    Assert( m_arrBuffer != NULL );
    return _SAI_Length();
}
template<typename T>
inline UInt _DenseArray<T>::SizeAllocated() const {
    Assert( m_arrBuffer != NULL );
    return m_iAllocatedSize;
}

template<typename T>
inline Void _DenseArray<T>::Clear() {
    Assert( m_arrBuffer != NULL );
    _SAI_SetNew( 0 );
}

template<typename T>
inline Void _DenseArray<T>::Affect( const T * arrItems, UInt iLength ) {
    Assert( m_arrBuffer != NULL );
    _GAA_DeleteNew( _GAA_AlignSize(iLength) );
    _SAI_SetNew( iLength );
    if (arrItems != NULL)
        MemCopy( m_arrBuffer + _SAI_Begin(), arrItems, iLength * sizeof(T) );
}
template<typename T>
Void _DenseArray<T>::EnsureCapacity( UInt iRequiredSize, Bool bShrinkTo )
{
    Assert( m_arrBuffer != NULL );

    m_iShrinkThreshold = INVALID_OFFSET; // disable shrinking

    UInt iTargetSize = _GAA_AlignSize(iRequiredSize);
    if ( iTargetSize == m_iAllocatedSize )
        return;
    if ( iTargetSize > m_iAllocatedSize ) {
        Int iDeltaPages = (Int)( (iTargetSize - m_iAllocatedSize) / m_iPageSize );
        _GAT_Resize( +iDeltaPages );
    } else { // iTargetSize < m_iAllocatedSize
        if (bShrinkTo) {
            Int iDeltaPages = (Int)( (m_iAllocatedSize - iTargetSize) / m_iPageSize );
            _GAT_Resize( -iDeltaPages );
        }
    }
}
template<typename T>
Void _DenseArray<T>::SetShrinking( Bool bEnable, UInt iThreshold )
{
    Assert( m_arrBuffer != NULL );

    if (bEnable) {
        m_iShrinkThreshold = m_iPageSize;
        if (iThreshold == INVALID_OFFSET)
            m_iShrinkThreshold += (m_iPageSize >> 1);
            m_iShrinkThreshold += (m_iPageSize >> 2);
        else {
            Assert(iThreshold > 0);
            Assert(iThreshold < m_iPageSize);
            m_iShrinkThreshold += iThreshold;
        }
    } else
        m_iShrinkThreshold = INVALID_OFFSET;
}

template<typename T>
UInt _DenseArray<T>::Search( const T & rItem, UInt iAt, UInt iCount ) const
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = _SAI_Length();
    Assert(iAt <= iLength);
    if (iCount == INVALID_OFFSET)
        iCount = iLength - iAt;
    Assert( iAt + iCount <= iLength );
    UInt iBegin = _SAI_Begin();

    T * pRange = m_arrBuffer + iBegin + iAt;
    T * pRangeEnd = pRange + iCount;
	while(pRange < pRangeEnd) {
		if ( *pRange == rItem )
			return ( pRange - (m_arrBuffer + iBegin) );
        ++pRange;
	}
	return INVALID_OFFSET;
}
template<typename T>
UInt _DenseArray<T>::Search( Comparator pfCompare, const T & rItem, UInt iAt, UInt iCount ) const
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = _SAI_Length();
    Assert(iAt <= iLength);
    if (iCount == INVALID_OFFSET)
        iCount = iLength - iAt;
    Assert( iAt + iCount <= iLength );
    UInt iBegin = _SAI_Begin();

    T * pRange = m_arrBuffer + iBegin + iAt;
    T * pPivot = pRange + (iCount >> 1);
    Int iCmp;
    while(iCount > 0) {
        iCmp = pfCompare( *pPivot, rItem );
        if ( iCmp == 0 )
            return ( pPivot - (m_arrBuffer + iBegin) );
        if ( iCmp < 0 ) {
            iCount = ( pPivot - pRange );
        } else { // ( iCmp > 0 )
            iCount -= ( pPivot - pRange + 1 );
            pRange = pPivot + 1;
        }
        pPivot = pRange + (iCount >> 1);
    }
    return ( pPivot - (m_arrBuffer + iBegin) );
}
template<typename T>
Void _DenseArray<T>::QuickSort( Comparator pfCompare, UInt iAt, UInt iCount )
{
    Assert( m_arrBuffer != NULL );

    UInt iLength = _SAI_Length();
    Assert(iAt <= iLength);
    if (iCount == INVALID_OFFSET)
        iCount = iLength - iAt;
    Assert( iAt + iCount <= iLength );
    UInt iBegin = _SAI_Begin();

    static Bool s_bSendLeft = true;
    if (iCount <= 1)
        return;

    T * pRange = m_arrBuffer + iBegin + iAt;
    T * pPivot = pRange;
    T * pRangeBack = pRange + iCount - 1;
    Int iCmp;
    while( pPivot < pRangeBack ) {
        iCmp = pfCompare( *pPivot, *(pPivot + 1) );
        if ( iCmp < 0 ) {
			Swap<T>( pPivot, pPivot + 1 );
			++pPivot;
        } else if ( iCmp > 0 ) {
            if ( pPivot + 1 != pRangeBack )
			    Swap<T>( pPivot + 1, pRangeBack );
			--pRangeBack;
        } else { // ( iCmp == 0 )
            // balance distribution of equal cases to temper some annoying
            // cases like arrays filled with the same value ...
            if (s_bSendLeft) {
                Swap<T>( pPivot, pPivot + 1 );
				++pPivot;
			} else {
				if ( pPivot + 1 != pRangeBack )
			        Swap<T>( pPivot + 1, pRangeBack );
				--pRangeBack;
			}
			s_bSendLeft = !s_bSendLeft;
        }
    }

	UInt iLeft = ( pPivot - pRange );
	UInt iRight = ( iCount - iLeft - 1 );
	if (iLeft > 1)
		QuickSort( pfCompare, iAt, iLeft );
	if (iRight > 1)
		QuickSort( pfCompare, (pPivot + 1 - (m_arrBuffer + iBegin)), iRight );
}

template<typename T>
inline Void _DenseArray<T>::Push( const _DenseArray<T> & rArray, UInt iOffset, UInt iLength ) {
    Assert( iOffset < rArray.Length() );
    if (iLength == INVALID_OFFSET)
        iLength = (rArray.Length() - iOffset);
    Assert( iOffset <= (rArray.Length() - iLength) );
    Push( (const T*)(rArray) + iOffset, iLength );
}
template<typename T>
inline Void _DenseArray<T>::Unshift( const _DenseArray<T> & rArray, UInt iOffset, UInt iLength ) {
    Assert( iOffset < rArray.Length() );
    if (iLength == INVALID_OFFSET)
        iLength = (rArray.Length() - iOffset);
    Assert( iOffset <= (rArray.Length() - iLength) );
    Unshift( (const T*)(rArray) + iOffset, iLength );
}
template<typename T>
inline Void _DenseArray<T>::Insert( UInt iAt, const _DenseArray<T> & rArray, UInt iOffset, UInt iLength ) {
    Assert( iOffset < rArray.Length() );
    if (iLength == INVALID_OFFSET)
        iLength = (rArray.Length() - iOffset);
    Assert( iOffset <= (rArray.Length() - iLength) );
    Insert( iAt, (const T*)(rArray) + iOffset, iLength );
}
template<typename T>
inline Void _DenseArray<T>::Pop( _DenseArray<T> & outArray, UInt iLength ) {
    outArray.Affect( NULL, iLength );
    Pop( (T*)(outArray), iLength );
}
template<typename T>
inline Void _DenseArray<T>::Shift( _DenseArray<T> & outArray, UInt iLength ) {
    outArray.Affect( NULL, iLength );
    Shift( (T*)(outArray), iLength );
}
template<typename T>
inline Void _DenseArray<T>::Remove( UInt iAt, _DenseArray<T> & outArray, UInt iLength ) {
    outArray.Affect( NULL, iLength );
    Remove( iAt, (T*)(outArray), iLength );
}

/////////////////////////////////////////////////////////////////////////////////

// Generic Array Allocation (GAA)
template<typename T>
inline UInt _DenseArray<T>::_GAA_AlignSize( UInt iSize ) const {
    if (iSize < m_iPageSize)
        iSize = m_iPageSize;
    else {
        UInt mod = ( iSize & (m_iPageSize - 1) );
        if (mod > 0)
	        iSize += (m_iPageSize - mod);
    }
    return iSize;
}
template<typename T>
inline Void _DenseArray<T>::_GAA_InitNull() {
    m_iAllocatedSize = 0;
    m_arrBuffer = NULL;
}
template<typename T>
inline Void _DenseArray<T>::_GAA_InitNew( UInt iNewSize ) {
    Assert(iNewSize > 0);
    m_iAllocatedSize = iNewSize;

    m_arrBuffer = KNew() T[m_iAllocatedSize];

    Assert(m_arrBuffer != NULL);
}
template<typename T>
inline Void _DenseArray<T>::_GAA_DeleteNull() {
    if (m_arrBuffer != NULL)
        KDeleteA( m_arrBuffer );
    _GAA_InitNull();
}
template<typename T>
inline Void _DenseArray<T>::_GAA_DeleteNew( UInt iNewSize ) {
    if (m_arrBuffer != NULL)
        KDeleteA( m_arrBuffer );
    _GAA_InitNew( iNewSize );
}

// Specific Array Indexing (SAI)
////////////// Implemented by derived classes //////////////

// Generic Array Tranforms (GAT)
template<typename T>
Void _DenseArray<T>::_GAT_Resize( Int iPageDelta )
{
    Assert( m_arrBuffer != NULL );
    Assert( iPageDelta != 0 );

    T * pOldBuffer = m_arrBuffer;
    UInt iOldSize = m_iAllocatedSize;
    UInt iLength = _SAI_Length();
    UInt iOldBegin = _SAI_Begin();

    UInt iNewSize = iOldSize;
    iNewSize += ( iPageDelta * (signed)m_iPageSize );
    Assert( iLength <= iNewSize );
    _GAA_InitNew( iNewSize );
    _SAI_SetNew( iLength );
    UInt iNewBegin = _SAI_Begin();

    MemCopy( m_arrBuffer + iNewBegin, pOldBuffer + iOldBegin, iLength * sizeof(T) );

    KDeleteA( pOldBuffer );
}
template<typename T>
Void _DenseArray<T>::_GAT_ShiftExtend( Int iShift, UInt iSplitOffset )
{
    Assert( m_arrBuffer != NULL );
    Assert( iShift != 0 );
    UInt iAbsShift = (iShift < 0) ? -iShift : iShift;

    UInt iOldLength = _SAI_Length();
    UInt iOldBegin = _SAI_Begin();

    UInt iNewLength = (iOldLength + iAbsShift);
    UInt iNewBegin = iOldBegin;
    if (iShift < 0) { // Left
        iNewBegin -= iAbsShift;
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = iOldLength;
        // NEW[ (Split)* | Shift | (NewLen - Shift - Split)** ] = OLD[ (Split)* | (OldLen - Split)** ]
        MemCopy( m_arrBuffer + iNewBegin,
                 m_arrBuffer + iOldBegin,
                 iSplitOffset * sizeof(T) );
    } else { // Right
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = 0;
        // NEW[ (Split)** | Shift | (NewLen - Shift - Split)* ] = OLD[ (Split)** | (OldLen - Split)* ]
        MemCopyBackward( m_arrBuffer + iNewBegin + iSplitOffset + iAbsShift,
                         m_arrBuffer + iOldBegin + iSplitOffset,
                         (iOldLength - iSplitOffset) * sizeof(T) );
    }
}
template<typename T>
Void _DenseArray<T>::_GAT_ShiftDiscard( Int iShift, UInt iSplitOffset )
{
    Assert( m_arrBuffer != NULL );
    Assert( iShift != 0 );
    UInt iAbsShift = (iShift < 0) ? -iShift : iShift;

    UInt iOldLength = _SAI_Length();
    UInt iOldBegin = _SAI_Begin();

    UInt iNewLength = (iOldLength - iAbsShift);
    UInt iNewBegin = iOldBegin;
    if (iShift < 0) { // Left
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = 0;
        // NEW[ Split | (NewLen - Split)* ] = OLD[ Split | Shift | (OldLen - Shift - Split)* ]
        MemCopy( m_arrBuffer + iNewBegin + iSplitOffset,
                 m_arrBuffer + iOldBegin + iSplitOffset + iAbsShift,
                 (iNewLength - iSplitOffset) * sizeof(T) );
    } else { // Right
        iNewBegin += iAbsShift;
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = iOldLength - 1;
        // NEW[ (Split - Shift)* | (NewLen + Shift - Split)** ] = OLD[ (Split - Shift)* | Shift | (OldLen - Split)** ]
        MemCopyBackward( m_arrBuffer + iNewBegin,
                         m_arrBuffer + iOldBegin,
                         (iSplitOffset + 1 - iAbsShift) * sizeof(T) );
    }
}
template<typename T>
Void _DenseArray<T>::_GAT_ResizeAndShiftExtend( Int iPageDelta, Int iShift, UInt iSplitOffset )
{
    Assert( m_arrBuffer != NULL );
    Assert( iPageDelta != 0 );
    Assert( iShift != 0 );
    Int iAbsShift = (iShift < 0) ? -iShift : iShift;

    T * pOldBuffer = m_arrBuffer;
    UInt iOldSize = m_iAllocatedSize;
    UInt iOldLength = _SAI_Length();
    UInt iOldBegin = _SAI_Begin();

    UInt iNewSize = iOldSize;
    iNewSize += ( iPageDelta * (signed)m_iPageSize );
    _GAA_InitNew( iNewSize );
    UInt iNewLength = (iOldLength + iAbsShift);
    Assert( iNewLength <= iNewSize );
    _SAI_SetNew( iNewLength );
    UInt iNewBegin = _SAI_Begin();

    if (iShift < 0) { // Left
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = iOldLength;
        // NEW[ (Split)* | Shift | (NewLen - Shift - Split)** ] = OLD[ (Split)* | (OldLen - Split)** ]
        MemCopy( m_arrBuffer + iNewBegin,
                 pOldBuffer + iOldBegin,
                 iSplitOffset * sizeof(T) );
        MemCopy( m_arrBuffer + iNewBegin + iSplitOffset + iAbsShift,
                 pOldBuffer + iOldBegin + iSplitOffset,
                 (iOldLength - iSplitOffset) * sizeof(T) );
    } else { // Right
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = 0;
        // NEW[ (Split)* | Shift | (NewLen - Shift - Split)** ] = OLD[ (Split)* | (OldLen - Split)** ]
        MemCopy( m_arrBuffer + iNewBegin,
                 pOldBuffer + iOldBegin,
                 iSplitOffset * sizeof(T) );
        MemCopy( m_arrBuffer + iNewBegin + iSplitOffset + iAbsShift,
                 pOldBuffer + iOldBegin + iSplitOffset,
                 (iOldLength - iSplitOffset) * sizeof(T) );
    }

    KDeleteA( pOldBuffer );
}
template<typename T>
Void _DenseArray<T>::_GAT_ResizeAndShiftDiscard( Int iPageDelta, Int iShift, UInt iSplitOffset )
{
    Assert( m_arrBuffer != NULL );
    Assert( iPageDelta != 0 );
    Assert( iShift != 0 );
    Int iAbsShift = (iShift < 0) ? -iShift : iShift;

    T * pOldBuffer = m_arrBuffer;
    UInt iOldSize = m_iAllocatedSize;
    UInt iOldLength = _SAI_Length();
    UInt iOldBegin = _SAI_Begin();

    UInt iNewSize = iOldSize;
    iNewSize += ( iPageDelta * (signed)m_iPageSize );
    _GAA_InitNew( iNewSize );
    UInt iNewLength = (iOldLength - iAbsShift);
    Assert( iNewLength <= iNewSize );
    _SAI_SetNew( iNewLength );
    UInt iNewBegin = _SAI_Begin();

    if (iShift < 0) { // Left
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = 0;
        // NEW[ Split** | (NewLen - Split)* ] = OLD[ Split** | Shift | (OldLen - Shift - Split)* ]
        MemCopy( m_arrBuffer + iNewBegin,
                 pOldBuffer + iOldBegin,
                 iSplitOffset * sizeof(T) );
        MemCopy( m_arrBuffer + iNewBegin + iSplitOffset,
                 pOldBuffer + iOldBegin + iSplitOffset + iAbsShift,
                 (iNewLength - iSplitOffset) * sizeof(T) );
    } else { // Right
        if (iSplitOffset == INVALID_OFFSET)
            iSplitOffset = iOldLength - 1;
        // NEW[ (Split - Shift)* | (NewLen + Shift - Split)** ] = OLD[ (Split - Shift)* | Shift | (OldLen - Split)** ]
        MemCopy( m_arrBuffer + iNewBegin,
                 pOldBuffer + iOldBegin,
                 (iSplitOffset + 1 - iAbsShift) * sizeof(T) );
        MemCopy( m_arrBuffer + iNewBegin + iSplitOffset + 1 - iAbsShift,
                 pOldBuffer + iOldBegin + iSplitOffset + 1,
                 (iOldLength - iSplitOffset - 1) * sizeof(T) );
    }

    KDeleteA( pOldBuffer );
}

template<typename T>
inline Void _DenseArray<T>::_AutoGrow( UInt iSize ) {
    UInt iGrowSize = _SAI_Length() + iSize;
    if ( iGrowSize > m_iAllocatedSize ) {
        Int iDeltaPages = (Int)( _GAA_AlignSize(iGrowSize - m_iAllocatedSize) / m_iPageSize );
        _GAT_Resize( +iDeltaPages );
    }
}
template<typename T>
inline Void _DenseArray<T>::_AutoShrink() {
    if ( m_iShrinkThreshold != INVALID_OFFSET ) {
        UInt iShrinkSize = m_iAllocatedSize - _SAI_Length();
        if ( iShrinkSize > m_iShrinkThreshold ) {
            Int iDeltaPages = (Int)( iShrinkSize / m_iPageSize );
            if ( (iShrinkSize % m_iPageSize) == 0 )
                --iDeltaPages;
            _GAT_Resize( -iDeltaPages );
        }
    }
}
