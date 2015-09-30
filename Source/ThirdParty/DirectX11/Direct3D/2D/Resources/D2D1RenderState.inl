/////////////////////////////////////////////////////////////////////////////////
// File : ThirdParty/DirectX11/Direct3D/2D/Resources/D2D1RenderState.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : D2D1 Dev-Ind Resource : Render States.
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// D2D1RenderState implementation
inline Bool D2D1RenderState::IsCreated() const {
    return ( m_pStateBlock != NULL );
}

/////////////////////////////////////////////////////////////////////////////////
// D2D1TextRenderState implementation
inline Bool D2D1TextRenderState::IsCreated() const {
    return ( m_pTextRenderingParams != NULL );
}

