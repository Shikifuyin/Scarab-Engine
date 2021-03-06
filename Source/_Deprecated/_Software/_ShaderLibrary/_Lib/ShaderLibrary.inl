/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/ShaderLibrary.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Software implementation for lots of common-use shaders
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ShaderLibrary implementation
inline ShaderLibrary * ShaderLibrary::GetInstancePtr() {
    static ShaderLibrary s_Instance;
    return &s_Instance;
}

inline LibVertexShaderProgram * ShaderLibrary::GetVS( VShaderLibID idVS ) const {
    Assert(idVS < VSHADER_COUNT);
    return m_arrVShaders[idVS];
}
inline LibPixelShaderProgram * ShaderLibrary::GetPS( PShaderLibID idPS ) const {
    Assert(idPS < PSHADER_COUNT);
    return m_arrPShaders[idPS];
}
