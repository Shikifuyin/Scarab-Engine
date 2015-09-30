/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Resource/ResourceFile.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base interface for all resource-managed files.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ResourceFile implementation
inline Bool ResourceFile::HasData() const {
    return ( m_pData != NULL );
}
inline const ResourceFileHeader * ResourceFile::GetHeader() const {
    return m_pHeader;
}
inline Byte * ResourceFile::GetData() const {
    return m_pData;
}
