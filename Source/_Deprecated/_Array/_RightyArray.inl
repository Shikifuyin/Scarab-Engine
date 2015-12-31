/////////////////////////////////////////////////////////////////////////////////
// File : Lib/Core/Datastruct/Array/RightyArray.inl
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
// ArrayR Implementation
template<typename T>
ArrayR<T>::ArrayR():
    _DenseArray<T>()
{
    _SAI_SetNull();
}
template<typename T>
ArrayR<T>::ArrayR( UInt iPageSize ):
    _DenseArray<T>( iPageSize )
{
    _SAI_SetNew( 0 );
}
template<typename T>
ArrayR<T>::ArrayR( UInt iPageSize, const T & rItem ):
    _DenseArray<T>( iPageSize, rItem )
{
    _SAI_SetNew( 1 );
    m_arrBuffer[m_iBegin] = rItem;
}
template<typename T>
ArrayR<T>::ArrayR( UInt iPageSize, const T * arrItems, UInt iLength ):
    _DenseArray<T>( iPageSize, arrItems, iLength )
{
    _SAI_SetNew( iLength );
    MemCopy( m_arrBuffer + m_iBegin, arrItems, iLength * sizeof(T) );
}
template<typename T>
ArrayR<T>::~ArrayR()
{
	// nothing to do
}

template<typename T>
inline Void ArrayR<T>::Push( const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrowShift( 1 );
    m_arrBuffer[m_iAllocatedSize - 1] = rItem;
}
template<typename T>
inline Void ArrayR<T>::Push( const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrowShift( iCount );
    MemCopy( m_arrBuffer + m_iAllocatedSize - iCount, arrItems, iCount * sizeof(T) );
}
template<typename T>
inline Void ArrayR<T>::Unshift( const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrow( 1 );
    m_arrBuffer[m_iBegin - 1] = rItem;
    --m_iBegin;
}
template<typename T>
inline Void ArrayR<T>::Unshift( const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    _AutoGrow( iCount );
    MemCopy( m_arrBuffer + m_iBegin - iCount, arrItems, iCount * sizeof(T) );
    m_iBegin -= iCount;
}
template<typename T>
inline Void ArrayR<T>::Insert( UInt iAt, const T & rItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= _SAI_Length() );
    _AutoGrowShift( 1, iAt );
    m_arrBuffer[m_iBegin + iAt] = rItem;
}
template<typename T>
inline Void ArrayR<T>::Insert( UInt iAt, const T * arrItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( iAt <= _SAI_Length() );
    _AutoGrowShift( iCount, iAt );
    MemCopy( m_arrBuffer + m_iBegin + iAt, arrItems, iCount * sizeof(T) );
}

template<typename T>
inline Void ArrayR<T>::Pop( T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iAllocatedSize - 1 );
    outItem = m_arrBuffer[m_iAllocatedSize - 1];
    _AutoShrinkShift( 1 );
}
template<typename T>
inline Void ArrayR<T>::Pop( T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iAllocatedSize - iCount );
    if (outItems != NULL)
        MemCopy( outItems, m_arrBuffer + m_iAllocatedSize - iCount, iCount * sizeof(T) );
    _AutoShrinkShift( iCount );
}
template<typename T>
inline Void ArrayR<T>::Shift( T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iAllocatedSize - 1 );
    outItem = m_arrBuffer[m_iBegin];
    ++m_iBegin;
    _AutoShrink();
}
template<typename T>
inline Void ArrayR<T>::Shift( T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iAllocatedSize - iCount );
    if (outItems != NULL)
        MemCopy( outItems, m_arrBuffer + m_iBegin, iCount * sizeof(T) );
    m_iBegin += iCount;
    _AutoShrink();
}
template<typename T>
inline Void ArrayR<T>::Remove( UInt iAt, T & outItem ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iAllocatedSize - 1 );
    Assert( iAt <= (_SAI_Length() - 1) );
    outItem = m_arrBuffer[m_iBegin + iAt];
    _AutoShrinkShift( 1, iAt );
}
template<typename T>
inline Void ArrayR<T>::Remove( UInt iAt, T * outItems, UInt iCount ) {
    Assert( m_arrBuffer != NULL );
    Assert( m_iBegin <= m_iAllocatedSize - iCount );
    Assert( iAt <= (_SAI_Length() - iCount) );
    if (outItems != NULL)
        MemCopy( outItems, m_arrBuffer + m_iBegin + iAt, iCount * sizeof(T) );
    _AutoShrinkShift( iCount, iAt );
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline Void ArrayR<T>::_SAI_SetNull() {
    m_iBegin = INVALID_OFFSET;
}
template<typename T>
inline Void ArrayR<T>::_SAI_SetNew( UInt iLength ) {
    m_iBegin = m_iAllocatedSize - iLength;
}
template<typename T>
inline UInt ArrayR<T>::_SAI_Begin() const {
    return m_iBegin;
}
template<typename T>
inline UInt ArrayR<T>::_SAI_Length() const {
    return m_iAllocatedSize - m_iBegin;
}

template<typename T>
inline Void ArrayR<T>::_AutoGrowShift( UInt iSize, UInt iSplitOffset ) {
    UInt iGrowSize = _SAI_Length() + iSize;
    if ( iGrowSize > m_iAllocatedSize ) {
        Int iDeltaPages = (Int)( _GAA_AlignSize(iGrowSize - m_iAllocatedSize) / m_iPageSize );
        _GAT_ResizeAndShiftExtend( +iDeltaPages, -((Int)iSize), iSplitOffset );
    } else {
        _GAT_ShiftExtend( -((Int)iSize), iSplitOffset );
        m_iBegin -= iSize;
    }
}
template<typename T>
inline Void ArrayR<T>::_AutoShrinkShift( UInt iSize, UInt iSplitOffset ) {
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
            _GAT_ShiftDiscard( +((Int)iSize), iSplitOffset + (iSize - 1) );
        else
            _GAT_ShiftDiscard( +((Int)iSize) );
        m_iBegin += iSize;
    }
}
