/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/BalancedArray.inl
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
// ArrayB Implementation
template<typename T>
ArrayB<T>::ArrayB():
    _DenseArray<T>()
{
    _SAI_SetNull();
}
template<typename T>
ArrayB<T>::ArrayB( UInt iPageSize ):
    _DenseArray<T>( iPageSize )
{
    _SAI_SetNew( 0 );
}
template<typename T>
ArrayB<T>::ArrayB( UInt iPageSize, const T & rItem ):
    _DenseArray<T>( iPageSize, rItem )
{
    _SAI_SetNew( 1 );
    m_arrBuffer[m_iBegin] = rItem;
}
template<typename T>
ArrayB<T>::ArrayB( UInt iPageSize, const T * arrItems, UInt iLength ):
    _DenseArray<T>( iPageSize, arrItems, iLength )
{
    _SAI_SetNew( iLength );
    MemCopy( m_arrBuffer + m_iBegin, arrItems, iLength * sizeof(T) );
}
template<typename T>
ArrayB<T>::~ArrayB()
{
	// nothing to do
}

template<typename T>
inline Void ArrayB<T>::Push( const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    _RebalanceShiftGrowRight( 1 );
    m_arrBuffer[m_iNextEnd - 1] = rItem;
}
template<typename T>
inline Void ArrayB<T>::Push( const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    _RebalanceShiftGrowRight( iCount );
    MemCopy( m_arrBuffer + m_iNextEnd - iCount, arrItems, iCount * sizeof(T) );
}
template<typename T>
inline Void ArrayB<T>::Unshift( const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    _RebalanceShiftGrowLeft( 1 );
    m_arrBuffer[m_iBegin] = rItem;
}
template<typename T>
inline Void ArrayB<T>::Unshift( const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    _RebalanceShiftGrowLeft( iCount );
    MemCopy( m_arrBuffer + m_iBegin, arrItems, iCount * sizeof(T) );
}
template<typename T>
inline Void ArrayB<T>::Insert( UInt iAt, const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= _SAI_Length() );
    if (m_iBalance >= 0)
        _RebalanceShiftGrowLeft( 1, iAt );
    else
        _RebalanceShiftGrowRight( 1, iAt );
    m_arrBuffer[m_iBegin + iAt] = rItem;
}
template<typename T>
inline Void ArrayB<T>::Insert( UInt iAt, const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= _SAI_Length() );
    if (m_iBalance >= 0)
        _RebalanceShiftGrowLeft( iCount, iAt );
    else
        _RebalanceShiftGrowRight( iCount, iAt );
    MemCopy( m_arrBuffer + m_iBegin + iAt, arrItems, iCount * sizeof(T) );
}

template<typename T>
inline Void ArrayB<T>::Pop( T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= m_iBegin + 1 );
    outItem = m_arrBuffer[m_iNextEnd - 1];
    _RebalanceShiftShrinkRight( 1 );
}
template<typename T>
inline Void ArrayB<T>::Pop( T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= m_iBegin + iCount );
    if ( outItems != NULL )
        MemCopy( outItems, m_arrBuffer + m_iNextEnd - iCount, iCount * sizeof(T) );
    _RebalanceShiftShrinkRight( iCount );
}
template<typename T>
inline Void ArrayB<T>::Shift( T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iNextEnd - 1 );
    outItem = m_arrBuffer[m_iBegin];
    _RebalanceShiftShrinkLeft( 1 );
}
template<typename T>
inline Void ArrayB<T>::Shift( T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iNextEnd - iCount );
    if ( outItems != NULL )
        MemCopy( outItems, m_arrBuffer + m_iBegin, iCount * sizeof(T) );
    _RebalanceShiftShrinkLeft( iCount );
}
template<typename T>
inline Void ArrayB<T>::Remove( UInt iAt, T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= m_iBegin + 1 );
    Assert( iAt <= (_SAI_Length() - 1) );
    outItem = m_arrBuffer[m_iBegin + iAt];
    if (m_iBalance >= 0)
        _RebalanceShiftShrinkRight( 1, iAt );
    else
        _RebalanceShiftShrinkLeft( 1, iAt );
}
template<typename T>
inline Void ArrayB<T>::Remove( UInt iAt, T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= m_iBegin + iCount );
    Assert( iAt <= (_SAI_Length() - iCount) );
    if ( outItems != NULL )
        MemCopy( outItems, m_arrBuffer + m_iBegin + iAt, iCount * sizeof(T) );
    if (m_iBalance >= 0)
        _RebalanceShiftShrinkRight( iCount, iAt );
    else
        _RebalanceShiftShrinkLeft( iCount, iAt );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline Void ArrayB<T>::_SAI_SetNull() {
    m_iBegin = INVALID_OFFSET;
    m_iNextEnd = INVALID_OFFSET;
    m_iMiddle = INVALID_OFFSET;
    m_iLeftHeavy = 0;
    m_iRightHeavy = 0;
    m_iBalance = 0;
}
template<typename T>
inline Void ArrayB<T>::_SAI_SetNew( UInt iLength ) {
    m_iMiddle = (m_iAllocatedSize >> 1); // m_iAllocatedSize is allways even
    m_iNextEnd = m_iMiddle;
    m_iBegin = m_iMiddle;
    UInt iHalfLength = (iLength >> 1);
    m_iNextEnd += (iLength - iHalfLength);
    m_iBegin -= iHalfLength;
    m_iRightHeavy = m_iNextEnd - m_iMiddle;
    m_iLeftHeavy = m_iMiddle - m_iBegin;
    m_iBalance = (m_iRightHeavy - m_iLeftHeavy);
}
template<typename T>
inline UInt ArrayB<T>::_SAI_Begin() const {
    return m_iBegin;
}
template<typename T>
inline UInt ArrayB<T>::_SAI_Length() const {
    return m_iNextEnd - m_iBegin;
}

template<typename T>
Void ArrayB<T>::_RebalanceShiftGrowLeft( UInt iSize, UInt iSplitOffset )
{
    UInt iGrowSize = _SAI_Length() + iSize;
    if ( iGrowSize > m_iAllocatedSize ) {
        // Grow case
        Int iDeltaPages = (Int)( _GAA_AlignSize(iGrowSize - m_iAllocatedSize) / m_iPageSize );
        _GAT_ResizeAndShiftExtend( +iDeltaPages, +((Int)iSize), iSplitOffset );
    } else {
        UInt iLeftSize = m_iBegin;
        if ( iSize <= iLeftSize ) {
            if (iSplitOffset != INVALID_OFFSET)
                _GAT_ShiftExtend( -((Int)iSize), iSplitOffset );
            m_iBegin -= iSize;
        } else {
            if (iLeftSize > 0) {
                if (iSplitOffset != INVALID_OFFSET)
                    _GAT_ShiftExtend( -((Int)iLeftSize), iSplitOffset );
                m_iBegin -= iLeftSize;
            }
            UInt iRemainSize = (iSize - iLeftSize);
            UInt iRightSize = m_iAllocatedSize - m_iNextEnd;
            Assert( iRemainSize <= iRightSize ); // allways true, since iGrowSize <= m_iAllocatedSize
            Int iShift = (Int)iRemainSize;
            if (m_iBalance < 0)
                iShift += ((-m_iBalance) >> 1);
            Int iMaxShift = (m_iMiddle - m_iRightHeavy);
            if (iShift > iMaxShift)
                iShift = iMaxShift;
            _GAT_ShiftExtend( +iShift, iSplitOffset );
            m_iBegin += (iShift - iRemainSize);
            m_iNextEnd += iShift;
        }
        m_iRightHeavy = m_iNextEnd - m_iMiddle;
        m_iLeftHeavy = m_iMiddle - m_iBegin;
        m_iBalance = (m_iRightHeavy - m_iLeftHeavy);
    }
}
template<typename T>
Void ArrayB<T>::_RebalanceShiftGrowRight( UInt iSize, UInt iSplitOffset )
{
    UInt iGrowSize = _SAI_Length() + iSize;
    if ( iGrowSize > m_iAllocatedSize ) {
        // Grow case
        Int iDeltaPages = (Int)( _GAA_AlignSize(iGrowSize - m_iAllocatedSize) / m_iPageSize );
        _GAT_ResizeAndShiftExtend( +iDeltaPages, -((Int)iSize), iSplitOffset );
    } else {
        UInt iRightSize = m_iAllocatedSize - m_iNextEnd;
        if ( iSize <= iRightSize ) {
            if (iSplitOffset != INVALID_OFFSET)
                _GAT_ShiftExtend( +((Int)iSize), iSplitOffset );
            m_iNextEnd += iSize;
        } else {
            if (iRightSize > 0) {
                if (iSplitOffset != INVALID_OFFSET)
                    _GAT_ShiftExtend( +((Int)iRightSize), iSplitOffset );
                m_iNextEnd += iRightSize;
            }
            UInt iRemainSize = (iSize - iRightSize);
            UInt iLeftSize = m_iBegin;
            Assert( iRemainSize <= iLeftSize ); // allways true, since iGrowSize <= m_iAllocatedSize
            Int iShift = (Int)iRemainSize;
            if (m_iBalance > 0)
                iShift += (m_iBalance >> 1);
            Int iMaxShift = (m_iMiddle - m_iLeftHeavy);
            if (iShift > iMaxShift)
                iShift = iMaxShift;
            _GAT_ShiftExtend( -iShift, iSplitOffset );
            m_iBegin -= iShift;
            m_iNextEnd -= (iShift - iRemainSize);
        }
        m_iRightHeavy = m_iNextEnd - m_iMiddle;
        m_iLeftHeavy = m_iMiddle - m_iBegin;
        m_iBalance = (m_iRightHeavy - m_iLeftHeavy);
    }
}

template<typename T>
Void ArrayB<T>::_RebalanceShiftShrinkLeft( UInt iSize, UInt iSplitOffset )
{
    UInt iShrinkSize = m_iAllocatedSize - (_SAI_Length() - iSize);
    if ( m_iShrinkThreshold != INVALID_OFFSET && iShrinkSize > m_iShrinkThreshold ) {
        Int iDeltaPages = (Int)( iShrinkSize / m_iPageSize );
        if ( (iShrinkSize % m_iPageSize) == 0 )
            --iDeltaPages;
        if (iSplitOffset != INVALID_OFFSET)
            _GAT_ResizeAndShiftDiscard( -iDeltaPages, -((Int)iSize), iSplitOffset );
        else
            _GAT_ResizeAndShiftDiscard( -iDeltaPages, -((Int)iSize) );
    } else {
        if (iSplitOffset != INVALID_OFFSET)
            _GAT_ShiftDiscard( +((Int)iSize), iSplitOffset + (iSize - 1) );
        m_iBegin += iSize;
        if (m_iBegin > m_iMiddle) {
            Int iShift = (m_iBegin - m_iMiddle);
            if (m_iBalance > 0)
                iShift += (m_iBalance >> 1);
            _GAT_ShiftExtend( -iShift );
            m_iBegin -= iShift;
            m_iNextEnd -= iShift;
        }
        m_iRightHeavy = m_iNextEnd - m_iMiddle;
        m_iLeftHeavy = m_iMiddle - m_iBegin;
        m_iBalance = (m_iRightHeavy - m_iLeftHeavy);
    }
}
template<typename T>
Void ArrayB<T>::_RebalanceShiftShrinkRight( UInt iSize, UInt iSplitOffset )
{
    UInt iShrinkSize = m_iAllocatedSize - (_SAI_Length() - iSize);
    if ( m_iShrinkThreshold != INVALID_OFFSET && iShrinkSize > m_iShrinkThreshold ) {
        Int iDeltaPages = (Int)( iShrinkSize / m_iPageSize );
        if ( (iShrinkSize % m_iPageSize) == 0 )
            --iDeltaPages;
        if (iSplitOffset != INVALID_OFFSET)
            _GAT_ResizeAndShiftDiscard( -iDeltaPages, +((Int)iSize), iSplitOffset + (iSize - 1) );
        else
            _GAT_ResizeAndShiftDiscard( -iDeltaPages, +((Int)iSize) );
    } else {
        if (iSplitOffset != INVALID_OFFSET)
            _GAT_ShiftDiscard( -((Int)iSize), iSplitOffset );
        m_iNextEnd -= iSize;
        if (m_iNextEnd < m_iMiddle) {
            Int iShift = (m_iMiddle - m_iNextEnd);
            if (m_iBalance < 0)
                iShift += ((-m_iBalance) >> 1);
            _GAT_ShiftExtend( iShift );
            m_iBegin += iShift;
            m_iNextEnd += iShift;
        }
        m_iRightHeavy = m_iNextEnd - m_iMiddle;
        m_iLeftHeavy = m_iMiddle - m_iBegin;
        m_iBalance = (m_iRightHeavy - m_iLeftHeavy);
    }
}
