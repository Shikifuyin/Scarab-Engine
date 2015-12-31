/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Shader.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Basic classes to handle shader programs management ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Shader*Constant implementations
inline const KChar * ShaderRenderingConstant::GetName( RenderingConstantType iType ) {
    return sm_arrTypeToString[iType];
}

inline RenderingConstantType ShaderRenderingConstant::GetType() const {
    return m_iType;
}
inline UInt ShaderRenderingConstant::GetBaseRegister() const {
    return m_iBaseRegister;
}
inline UInt ShaderRenderingConstant::GetRegisterCount() const {
    return m_iRegisterCount;
}

inline UInt ShaderNumericConstant::GetRegister() const {
    return m_iRegister;
}
inline Scalar * ShaderNumericConstant::GetData() const {
    return (Scalar*)m_arrData;
}

inline const KChar * ShaderUserConstant::GetName() const {
    return m_strName;
}
inline UInt ShaderUserConstant::GetBaseRegister() const {
    return m_iBaseRegister;
}
inline UInt ShaderUserConstant::GetRegisterCount() const {
    return m_iRegisterCount;
}

inline const KChar * ShaderSamplerDescriptor::GetName() const {
    return m_strName;
}
inline SamplerType ShaderSamplerDescriptor::GetType() const {
    return m_iType;
}
inline UInt ShaderSamplerDescriptor::GetTextureChannel() const {
    return m_iTextureChannel;
}
inline UInt ShaderSamplerDescriptor::GetDimension() const {
    return m_iDimension;
}

/////////////////////////////////////////////////////////////////////////////////
// ShaderProgram implementation
inline CustomFragmentLayout * ShaderProgram::GetInputCFL() const {
    return m_pInputCFL;
}
inline CustomFragmentLayout * ShaderProgram::GetOutputCFL() const {
    return m_pOutputCFL;
}

inline UInt ShaderProgram::GetRenderingConstantCount() const {
    return m_arrRenderingConstants.Length();
}
inline ShaderRenderingConstant * ShaderProgram::GetRenderingConstant( UInt iIndex ) const {
    Assert(iIndex < m_arrRenderingConstants.Length());
    return (ShaderRenderingConstant*)( &(m_arrRenderingConstants[iIndex]) );
}
inline UInt ShaderProgram::GetNumericConstantCount() const {
    return m_arrNumericConstants.Length();
}
inline ShaderNumericConstant * ShaderProgram::GetNumericConstant( UInt iIndex ) const {
    Assert(iIndex < m_arrNumericConstants.Length());
    return (ShaderNumericConstant*)( &(m_arrNumericConstants[iIndex]) );
}
inline UInt ShaderProgram::GetUserConstantCount() const {
    return m_arrUserConstants.Length();
}
inline ShaderUserConstant * ShaderProgram::GetUserConstant( UInt iIndex ) const {
    Assert(iIndex < m_arrUserConstants.Length());
    return (ShaderUserConstant*)( &(m_arrUserConstants[iIndex]) );
}
inline UInt ShaderProgram::GetSamplerDescCount() const {
    return m_arrSamplerDescriptors.Length();
}
inline ShaderSamplerDescriptor * ShaderProgram::GetSamplerDesc( UInt iIndex ) const {
    Assert(iIndex < m_arrSamplerDescriptors.Length());
    return (ShaderSamplerDescriptor*)( &(m_arrSamplerDescriptors[iIndex]) );
}

inline const KChar * ShaderProgram::GetCodeText() const {
    return m_strCodeText;
}
