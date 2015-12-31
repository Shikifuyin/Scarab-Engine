/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/ShaderLibrary/Default.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Default very-very-basic shaders tu ensure minimal functions
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
inline VShaderLibID VertexShader_Default_Null::GetID() const      { return VSHADER_DEFAULT_NULL; }
inline VShaderLibID VertexShader_Default_Color::GetID() const     { return VSHADER_DEFAULT_COLOR; }
inline VShaderLibID VertexShader_Default_Texture1D::GetID() const { return VSHADER_DEFAULT_TEXTURE1D; }
inline VShaderLibID VertexShader_Default_Texture2D::GetID() const { return VSHADER_DEFAULT_TEXTURE2D; }

/////////////////////////////////////////////////////////////////////////////////
// Pixel Shaders
inline PShaderLibID PixelShader_Default_Null::GetID() const      { return PSHADER_DEFAULT_NULL; }
inline PShaderLibID PixelShader_Default_Color::GetID() const     { return PSHADER_DEFAULT_COLOR; }
inline PShaderLibID PixelShader_Default_Texture1D::GetID() const { return PSHADER_DEFAULT_TEXTURE1D; }
inline PShaderLibID PixelShader_Default_Texture2D::GetID() const { return PSHADER_DEFAULT_TEXTURE2D; }
