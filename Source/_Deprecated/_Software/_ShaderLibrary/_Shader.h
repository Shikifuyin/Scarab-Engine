/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Shader.h
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
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Shader Language Grammar
// <Code> = <CodeLine> <Code>
//        | <CodeLine>
// <CodeLine> = <Comment> | <Declaration> | <Instruction>
// <Comment> = "; <text> \n"
// <Declaration> = <DeclConst> | <DeclVar>
// <DeclConst> = "#const c[<index>] = <float> <float> <float> <float>\n"
// <DeclVar> = "#var <type> <texunit>? <name> <DeclVarStorage>?\n"
// <DeclVarStorage> = "&(in|out).<semantic>"
//                  | "c[<index>]"
// <Instruction> = <Instruction0> | <Instruction1> | <Instruction2> | <Instruction3>
// <Instruction0> = "<mnemonic>\n"
// <Instruction1> = "<mnemonic> <LOperand>\n"
// <Instruction2> = "<mnemonic> <LOperand>, <ROperand>\n"
// <Instruction3> = "<mnemonic> <LOperand>, <ROperand>, <ROperand>\n"
// <LOperand> = ...
// <ROperand> = ...

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef KOALA_ENGINE_GRAPHICS_3D_SHADER_H
#define KOALA_ENGINE_GRAPHICS_3D_SHADER_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../../Lib/Platform/File/File.h"

#include "../../../Lib/Core/Datastruct/Array/LeftyArray.h"

#include "Buffering.h"
#include "Sampler.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
class Renderer;

enum RenderingConstantType
{
    RENDERING_CONST_UNDEFINED = 0,

    RENDERING_CONST_MATRIX_WORLD,         // 4x4 local-to-world
    RENDERING_CONST_MATRIX_VIEW,          // 4x4 world-to-view
    RENDERING_CONST_MATRIX_PROJ,          // 4x4 view-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEW,     // 4x4 local-to-view
    RENDERING_CONST_MATRIX_VIEWPROJ,      // 4x4 world-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEWPROJ, // 4x4 local-to-clip

    RENDERING_CONST_MATRIX_PROJECTOR,     // 4x4 world-to-clip

    RENDERING_CONST_MATRIX_WORLD_TRANSPOSE,         // 4x4 trans local-to-world
    RENDERING_CONST_MATRIX_VIEW_TRANSPOSE,          // 4x4 trans world-to-view
    RENDERING_CONST_MATRIX_PROJ_TRANSPOSE,          // 4x4 trans view-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEW_TRANSPOSE,     // 4x4 trans local-to-view
    RENDERING_CONST_MATRIX_VIEWPROJ_TRANSPOSE,      // 4x4 trans world-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEWPROJ_TRANSPOSE, // 4x4 trans local-to-clip

    RENDERING_CONST_MATRIX_WORLD_INVERSE,         // 4x4 inv local-to-world
    RENDERING_CONST_MATRIX_VIEW_INVERSE,          // 4x4 inv world-to-view
    RENDERING_CONST_MATRIX_PROJ_INVERSE,          // 4x4 inv view-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEW_INVERSE,     // 4x4 inv local-to-view
    RENDERING_CONST_MATRIX_VIEWPROJ_INVERSE,      // 4x4 inv world-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEWPROJ_INVERSE, // 4x4 inv local-to-clip

    RENDERING_CONST_MATRIX_WORLD_INVERSE_TRANSPOSE,         // 4x4 inv-trans local-to-world
    RENDERING_CONST_MATRIX_VIEW_INVERSE_TRANSPOSE,          // 4x4 inv-trans world-to-view
    RENDERING_CONST_MATRIX_PROJ_INVERSE_TRANSPOSE,          // 4x4 inv-trans view-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEW_INVERSE_TRANSPOSE,     // 4x4 inv-trans local-to-view
    RENDERING_CONST_MATRIX_VIEWPROJ_INVERSE_TRANSPOSE,      // 4x4 inv-trans world-to-clip
    RENDERING_CONST_MATRIX_WORLDVIEWPROJ_INVERSE_TRANSPOSE, // 4x4 inv-trans local-to-clip

    RENDERING_CONST_CAMERA_POSITION,  // 4 Floats (x,y,z,1)
    RENDERING_CONST_CAMERA_DIRECTION, // 4 Floats (x,y,z,0)
    RENDERING_CONST_CAMERA_UP,        // 4 Floats (x,y,z,0)
    RENDERING_CONST_CAMERA_RIGHT,     // 4 Floats (x,y,z,0)

    RENDERING_CONST_PROJECTOR_POSITION,  // 4 Floats (x,y,z,1)
    RENDERING_CONST_PROJECTOR_DIRECTION, // 4 Floats (x,y,z,0)
    RENDERING_CONST_PROJECTOR_UP,        // 4 Floats (x,y,z,0)
    RENDERING_CONST_PROJECTOR_RIGHT,     // 4 Floats (x,y,z,0)

    RENDERING_CONST_MATERIAL0_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL0_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL0_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL0_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL1_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL1_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL1_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL1_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL2_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL2_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL2_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL2_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL3_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL3_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL3_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL3_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL4_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL4_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL4_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL4_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL5_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL5_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL5_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL5_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL6_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL6_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL6_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL6_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_MATERIAL7_EMISSIVE, // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL7_AMBIENT,  // 4 Floats (r,g,b,1)
    RENDERING_CONST_MATERIAL7_DIFFUSE,  // 4 Floats (r,g,b,alpha)
    RENDERING_CONST_MATERIAL7_SPECULAR, // 4 Floats (r,g,b,exp)

    RENDERING_CONST_LIGHT0_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT0_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT0_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT0_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT0_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT0_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT0_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT1_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT1_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT1_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT1_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT1_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT1_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT1_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT2_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT2_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT2_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT2_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT2_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT2_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT2_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT3_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT3_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT3_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT3_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT3_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT3_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT3_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT4_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT4_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT4_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT4_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT4_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT4_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT4_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT5_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT5_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT5_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT5_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT5_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT5_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT5_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT6_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT6_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT6_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT6_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT6_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT6_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT6_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_LIGHT7_POSITION,    // 4 Floats (x,y,z,1)
    RENDERING_CONST_LIGHT7_DIRECTION,   // 4 Floats (x,y,z,0)
    RENDERING_CONST_LIGHT7_AMBIENT,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT7_DIFFUSE,     // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT7_SPECULAR,    // 4 Floats (r,g,b,a) (a = alpha)
    RENDERING_CONST_LIGHT7_ATTENUATION, // 4 Floats (const, linear, quadratic, intensity)
    RENDERING_CONST_LIGHT7_SPOTCUTOFF,  // 4 Floats (angle, sin, cos, exp)

    RENDERING_CONST_COUNT
};

    // ShaderProgram stuff
#define SHADER_REGISTER_CHANNELS 4
#define SHADER_MAX_SAMPLERS      16
#define SHADER_MAX_IDENT_SIZE    64
#define SHADER_MAX_CODE_SIZE     4096

/////////////////////////////////////////////////////////////////////////////////
// The Shader*Constant classes
class ShaderRenderingConstant
{
public:
    inline static const KChar * GetName( RenderingConstantType iType );
    static RenderingConstantType GetType( const KChar * strName );

    ShaderRenderingConstant();
    ShaderRenderingConstant( RenderingConstantType iType, UInt iBaseRegister, UInt iRegisterCount );
    ~ShaderRenderingConstant();

    inline RenderingConstantType GetType() const;
    inline UInt GetBaseRegister() const;
    inline UInt GetRegisterCount() const;

private:
    static const KChar * sm_arrTypeToString[RENDERING_CONST_COUNT];

    RenderingConstantType m_iType;
    UInt m_iBaseRegister;
    UInt m_iRegisterCount; // 1 or 4
};

class ShaderNumericConstant
{
public:
    ShaderNumericConstant();
    ShaderNumericConstant( UInt iRegister, Scalar arrValues[4] );
    ~ShaderNumericConstant();

    inline UInt GetRegister() const;
    inline Scalar * GetData() const;

private:
    UInt m_iRegister;
    Scalar m_arrData[4];
};

class ShaderUserConstant
{
public:
    ShaderUserConstant();
    ShaderUserConstant( const KChar * strName, UInt iBaseRegister, UInt iRegisterCount );
    ~ShaderUserConstant();

    inline const KChar * GetName() const;
    inline UInt GetBaseRegister() const;
    inline UInt GetRegisterCount() const;

private:
    KChar m_strName[SHADER_MAX_IDENT_SIZE];
    UInt m_iBaseRegister;
    UInt m_iRegisterCount;
};

class ShaderSamplerDescriptor
{
public:
    ShaderSamplerDescriptor();
    ShaderSamplerDescriptor( const KChar * strName, SamplerType iType, UInt iTextureChannel );
    ~ShaderSamplerDescriptor();

    inline const KChar * GetName() const;
    inline SamplerType GetType() const;
    inline UInt GetTextureChannel() const;
    inline UInt GetDimension() const;

private:
    KChar m_strName[SHADER_MAX_IDENT_SIZE];
    SamplerType m_iType;
    UInt m_iTextureChannel;
    UInt m_iDimension;
};

/////////////////////////////////////////////////////////////////////////////////
// The ShaderProgram interface
class ShaderProgram
{
public:
    virtual ~ShaderProgram();

    // Getters
        // Input & output formats
    inline CustomFragmentLayout * GetInputCFL() const;
    inline CustomFragmentLayout * GetOutputCFL() const;

        // Constants
    inline UInt GetRenderingConstantCount() const;
    inline ShaderRenderingConstant * GetRenderingConstant( UInt iIndex ) const;
    ShaderRenderingConstant * GetRenderingConstant( RenderingConstantType iType ) const;

    inline UInt GetNumericConstantCount() const;
    inline ShaderNumericConstant * GetNumericConstant( UInt iIndex ) const;

    inline UInt GetUserConstantCount() const;
    inline ShaderUserConstant * GetUserConstant( UInt iIndex ) const;
    ShaderUserConstant * GetUserConstant( const KChar * strName ) const;

    inline UInt GetSamplerDescCount() const;
    inline ShaderSamplerDescriptor * GetSamplerDesc( UInt iIndex ) const;
    ShaderSamplerDescriptor * GetSamplerDesc( const KChar * strName ) const;

        // Program code
    inline const KChar * GetCodeText() const;

protected:
    ShaderProgram();

    // Loaders
    static Bool _LoadFromText( const KChar * strProgramText, ShaderProgram * pOutProgram );
    static Bool _LoadFromFile( const KChar * strFilename, ShaderProgram * pOutProgram );

    // Parsing helpers
    static Bool _ParseDeclaration( const KChar * strLine, ShaderProgram * pOutProgram );
    static Bool _ParseDeclaration_Const( const KChar * strLine, ShaderProgram * pOutProgram );
    static Bool _ParseDeclaration_Var( const KChar * strLine, ShaderProgram * pOutProgram );
    static Bool _ParseDeclaration_Var_IO( const KChar * strLine, ShaderProgram * pOutProgram );
    static Bool _ParseDeclaration_Var_Reg( const KChar * strLine, ShaderProgram * pOutProgram );

    // Fills a register-buffer with all data
    Void _Prepare( Scalar * arrRegisters, Sampler ** arrSamplers ) const;

    // Data
    CustomFragmentLayout * m_pInputCFL;
    CustomFragmentLayout * m_pOutputCFL;

    ArrayL<ShaderRenderingConstant> m_arrRenderingConstants;
    ArrayL<ShaderNumericConstant> m_arrNumericConstants;
    ArrayL<ShaderUserConstant> m_arrUserConstants;
    ArrayL<ShaderSamplerDescriptor> m_arrSamplerDescriptors;

    KChar m_strCodeText[SHADER_MAX_CODE_SIZE];

    Bool m_bLoaded;

    // Parsing strings
    static const KChar sm_Token_LineDelimiter;
    static const KChar sm_Token_Delimiter;
    static const KChar sm_Token_Comment;
    static const KChar sm_Token_Declaration;
    static const KChar sm_Token_RegConstant;
    static const KChar sm_Token_IOConstant;

    static const KChar * sm_Declare_Const;
    static const KChar * sm_Declare_Var;

    static const KChar * sm_Type_Float;
    static const KChar * sm_Type_Float1;
    static const KChar * sm_Type_Float1x1;
    static const KChar * sm_Type_Float1x2;
    static const KChar * sm_Type_Float1x3;
    static const KChar * sm_Type_Float1x4;
    static const KChar * sm_Type_Float2;
    static const KChar * sm_Type_Float2x1;
    static const KChar * sm_Type_Float2x2;
    static const KChar * sm_Type_Float2x3;
    static const KChar * sm_Type_Float2x4;
    static const KChar * sm_Type_Float3;
    static const KChar * sm_Type_Float3x1;
    static const KChar * sm_Type_Float3x2;
    static const KChar * sm_Type_Float3x3;
    static const KChar * sm_Type_Float3x4;
    static const KChar * sm_Type_Float4;
    static const KChar * sm_Type_Float4x1;
    static const KChar * sm_Type_Float4x2;
    static const KChar * sm_Type_Float4x3;
    static const KChar * sm_Type_Float4x4;
    static const KChar * sm_Type_Sampler1D;
    static const KChar * sm_Type_Sampler2D;
    static const KChar * sm_Type_Sampler3D;
    static const KChar * sm_Type_SamplerCube;
    static const KChar * sm_Type_SamplerProj;

    static const KChar * sm_IOStatus_In;
    static const KChar * sm_IOStatus_Out;

    static const KChar * sm_Semantic_Position;
    static const KChar * sm_Semantic_RasterPos;
    static const KChar * sm_Semantic_Depth;
    static const KChar * sm_Semantic_InvertW;
    static const KChar * sm_Semantic_RegularW;
    static const KChar * sm_Semantic_Normal;
    static const KChar * sm_Semantic_Tangent;
    static const KChar * sm_Semantic_BiNormal;
    static const KChar * sm_Semantic_TexCoord;
    static const KChar * sm_Semantic_Color;
    static const KChar * sm_Semantic_Diffuse;
    static const KChar * sm_Semantic_Specular;
    static const KChar * sm_Semantic_BlendWeight;
    static const KChar * sm_Semantic_BlendIndices;
    static const KChar * sm_Semantic_FogCoord;
    static const KChar * sm_Semantic_FogFactor;
    static const KChar * sm_Semantic_Sample;
    static const KChar * sm_Semantic_PointSize;
    static const KChar * sm_Semantic_UserData;

    static KChar sm_strVarName[SHADER_MAX_IDENT_SIZE];
    static UInt sm_iFloatCount;
    static UInt sm_iRegisterCount;
};

/////////////////////////////////////////////////////////////////////////////////
// The VertexShaderProgram class
class VertexShaderProgram : public ShaderProgram
{
public:
    VertexShaderProgram( const KChar * strLoadString, Bool bLoadFromFile = true );
    virtual ~VertexShaderProgram();

    virtual Void Execute( CFLPtr pOutVertex, CCFLPtr pVertex, Scalar * arrRegisters ) = 0;

protected:
    friend class VertexShader;
};

/////////////////////////////////////////////////////////////////////////////////
// The PixelShaderProgram class
class PixelShaderProgram : public ShaderProgram
{
public:
    PixelShaderProgram( const KChar * strLoadString, Bool bLoadFromFile = true );
    virtual ~PixelShaderProgram();

    virtual Void Execute( Color4 & outColor, CCFLPtr pPixel, Scalar * arrRegisters, Sampler ** arrSamplers ) = 0;

protected:
    friend class PixelShader;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "Shader.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // KOALA_ENGINE_GRAPHICS_3D_SHADER_H

