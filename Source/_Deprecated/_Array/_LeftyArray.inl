/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/LeftyArray.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Dense-Array, Lefty behaviour.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ArrayL Implementation
template<typename T>
ArrayL<T>::ArrayL():
    _DenseArray<T>()
{
    _SAI_SetNull();
}
template<typename T>
ArrayL<T>::ArrayL( UInt iPageSize ):
    _DenseArray<T>( iPageSize )
{
    _SAI_SetNew( 0 );
}
template<typename T>
ArrayL<T>::ArrayL( UInt iPageSize, const T & rItem ):
    _DenseArray<T>( iPageSize, rItem )
{
    _SAI_SetNew( 1 );
    m_arrBuffer[0] = rItem;
}
template<typename T>
ArrayL<T>::ArrayL( UInt iPageSize, const T * arrItems, UInt iLength ):
    _DenseArray<T>( iPageSize, arrItems, iLength )
{
    _SAI_SetNew( iLength );
    MemCopy( m_arrBuffer, arrItems, iLength * sizeof(T) );
}
template<typename T>
ArrayL<T>::~ArrayL()
{
	// nothing to do
}

template<typename T>
inline Void ArrayL<T>::Push( const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrow( 1 );
    m_arrBuffer[m_iNextEnd] = rItem;
    ++m_iNextEnd;
}
template<typename T>
inline Void ArrayL<T>::Push( const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrow( iCount );
    MemCopy( m_arrBuffer + m_iNextEnd, arrItems, iCount * sizeof(T) );
    m_iNextEnd += iCount;
}
template<typename T>
inline Void ArrayL<T>::Unshift( const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrowShift( 1 );
    m_arrBuffer[0] = rItem;
}
template<typename T>
inline Void ArrayL<T>::Unshift( const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrowShift( iCount );
    MemCopy( m_arrBuffer, arrItems, iCount * sizeof(T) );
}
template<typename T>
inline Void ArrayL<T>::Insert( UInt iAt, const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= _SAI_Length() );
    _AutoGrowShift( 1, iAt );
    m_arrBuffer[iAt] = rItem;
}
template<typename T>
inline Void ArrayL<T>::Insert( UInt iAt, const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= _SAI_Length() );
    _AutoGrowShift( iCount, iAt );
    MemCopy( m_arrBuffer + iAt, arrItems, iCount * sizeof(T) );
}

template<typename T>
inline Void ArrayL<T>::Pop( T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= 1 );
    outItem = m_arrBuffer[m_iNextEnd - 1];
    --m_iNextEnd;
    _AutoShrink();
}
template<typename T>
inline Void ArrayL<T>::Pop( T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= iCount );
    if (outItems != NULL)
        MemCopy( outItems, m_arrBuffer + m_iNextEnd - iCount, iCount * sizeof(T) );
    m_iNextEnd -= iCount;
    _AutoShrink();
}
template<typename T>
inline Void ArrayL<T>::Shift( T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= 1 );
    outItem = m_arrBuffer[0];
    _AutoShrinkShift( 1 );
}
template<typename T>
inline Void ArrayL<T>::Shift( T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= iCount );
    if (outItems != NULL)
        MemCopy( outItems, m_arrBuffer, iCount * sizeof(T) );
    _AutoShrinkShift( iCount );
}
template<typename T>
inline Void ArrayL<T>::Remove( UInt iAt, T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= 1  );
    Assert( iAt <= (_SAI_Length() - 1) );
    outItem = m_arrBuffer[iAt];
    _AutoShrinkShift( 1, iAt );
}
template<typename T>
inline Void ArrayL<T>::Remove( UInt iAt, T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iNextEnd >= iCount );
    Assert( iAt <= (_SAI_Length() - iCount) );
    if (outItems != NULL)
        MemCopy( outItems, m_arrBuffer + iAt, iCount * sizeof(T) );
    _AutoShrinkShift( iCount, iAt );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline Void ArrayL<T>::_SAI_SetNull() {
    m_iNextEnd = INVALID_OFFSET;
}
template<typename T>
inline Void ArrayL<T>::_SAI_SetNew( UInt iLength ) {
    m_iNextEnd = iLength;
}
template<typename T>
inline UInt ArrayL<T>::_SAI_Begin() const {
    return 0;
}
template<typename T>
inline UInt ArrayL<T>::_SAI_Length() const {
    return m_iNextEnd;
}

template<typename T>
inline Void ArrayL<T>::_AutoGrowShift( UInt iSize, UInt iSplitOffset ) {
    UInt iGrowSize = _SAI_Length() + iSize;
    if ( iGrowSize > m_iAllocatedSize ) {
        Int iDeltaPages = (Int)( _GAA_AlignSize(iGrowSize - m_iAllocatedSize) / m_iPageSize );
        _GAT_ResizeAndShiftExtend( +iDeltaPages, +((Int)iSize), iSplitOffset );
    } else {
        _GAT_ShiftExtend( +((Int)iSize), iSplitOffset );
        m_iNextEnd += iSize;
    }
}
template<typename T>
inline Void ArrayL<T>::_AutoShrinkShift( UInt iSize, UInt iSplitOffset ) {
    UInt iShrinkSize = m_iAllocatedSize - (_SAI_Length() - iSize);
    if ( m_iShrinkThreshold != INVALID_OFFSET && iShrinkSize > m_iShrinkThreshold ) {
        Int iDeltaPages = (Int)( iShrinkSize / m_iPageSize );
        if ( (iShrinkSize % m_iPageSize) == 0 )
            --iDeltaPages;
        _GAT_ResizeAndShiftDiscard( -iDeltaPages, -((Int)iSize), iSplitOffset );
    } else {
        _GAT_ShiftDiscard( -((Int)iSize), iSplitOffset );
        m_iNextEnd -= iSize;
    }
}

