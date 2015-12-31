/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Transformer.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of the Renderer, handles transformations
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Transformer implementation
inline const Vertex3 & Transformer::GetViewEye() const {
    return m_vEye;
}
inline const Vector3 & Transformer::GetViewDir() const {
    return m_vView;
}
inline const Vector3 & Transformer::GetViewUp() const {
    return m_vUp;
}
inline const Vector3 & Transformer::GetViewRight() const {
    return m_vRight;
}
inline const Scalar * Transformer::GetFrustrum() const {
    return m_fFrustrum;
}
inline Scalar Transformer::GetFrustrumNear() const {
    return m_fFrustrum[FRUSTRUM_NEAR];
}
inline Scalar Transformer::GetFrustrumFar() const {
    return m_fFrustrum[FRUSTRUM_FAR];
}
inline Scalar Transformer::GetFrustrumBottom() const {
    return m_fFrustrum[FRUSTRUM_BOTTOM];
}
inline Scalar Transformer::GetFrustrumTop() const {
    return m_fFrustrum[FRUSTRUM_TOP];
}
inline Scalar Transformer::GetFrustrumLeft() const {
    return m_fFrustrum[FRUSTRUM_LEFT];
}
inline Scalar Transformer::GetFrustrumRight() const {
    return m_fFrustrum[FRUSTRUM_RIGHT];
}

