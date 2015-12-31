/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Shader.cpp
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
// Includes
#include "Shader.h"
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Shader*Constant implementations
const KChar * ShaderRenderingConstant::sm_arrTypeToString[RENDERING_CONST_COUNT] = {
    TEXT("<UNDEFINED>"),

    TEXT("WMatrix"),
    TEXT("VMatrix"),
    TEXT("PMatrix"),
    TEXT("WVMatrix"),
    TEXT("VPMatrix"),
    TEXT("WVPMatrix"),
    TEXT("ProjectorMatrix"),

    TEXT("WMatrixT"),
    TEXT("VMatrixT"),
    TEXT("PMatrixT"),
    TEXT("WVMatrixT"),
    TEXT("VPMatrixT"),
    TEXT("WVPMatrixT"),

    TEXT("WMatrixI"),
    TEXT("VMatrixI"),
    TEXT("PMatrixI"),
    TEXT("WVMatrixI"),
    TEXT("VPMatrixI"),
    TEXT("WVPMatrixI"),

    TEXT("WMatrixIT"),
    TEXT("VMatrixIT"),
    TEXT("PMatrixIT"),
    TEXT("WVMatrixIT"),
    TEXT("VPMatrixIT"),
    TEXT("WVPMatrixIT"),

    TEXT("CameraPosition"),
    TEXT("CameraDirection"),
    TEXT("CameraUp"),
    TEXT("CameraRight"),

    TEXT("ProjectorPosition"),
    TEXT("ProjectorDirection"),
    TEXT("ProjectorUp"),
    TEXT("ProjectorRight"),

    TEXT("Material0Emissive"),
    TEXT("Material0Ambient"),
    TEXT("Material0Diffuse"),
    TEXT("Material0Specular"),

    TEXT("Material1Emissive"),
    TEXT("Material1Ambient"),
    TEXT("Material1Diffuse"),
    TEXT("Material1Specular"),

    TEXT("Material2Emissive"),
    TEXT("Material2Ambient"),
    TEXT("Material2Diffuse"),
    TEXT("Material2Specular"),

    TEXT("Material3Emissive"),
    TEXT("Material3Ambient"),
    TEXT("Material3Diffuse"),
    TEXT("Material3Specular"),

    TEXT("Material4Emissive"),
    TEXT("Material4Ambient"),
    TEXT("Material4Diffuse"),
    TEXT("Material4Specular"),

    TEXT("Material5Emissive"),
    TEXT("Material5Ambient"),
    TEXT("Material5Diffuse"),
    TEXT("Material5Specular"),

    TEXT("Material6Emissive"),
    TEXT("Material6Ambient"),
    TEXT("Material6Diffuse"),
    TEXT("Material6Specular"),

    TEXT("Material7Emissive"),
    TEXT("Material7Ambient"),
    TEXT("Material7Diffuse"),
    TEXT("Material7Specular"),

    TEXT("Light0Position"),
    TEXT("Light0Direction"),
    TEXT("Light0Ambient"),
    TEXT("Light0Diffuse"),
    TEXT("Light0Specular"),
    TEXT("Light0Attenuation"),
    TEXT("Light0SpotCutoff"),

    TEXT("Light1Position"),
    TEXT("Light1Direction"),
    TEXT("Light1Ambient"),
    TEXT("Light1Diffuse"),
    TEXT("Light1Specular"),
    TEXT("Light1Attenuation"),
    TEXT("Light1SpotCutoff"),

    TEXT("Light2Position"),
    TEXT("Light2Direction"),
    TEXT("Light2Ambient"),
    TEXT("Light2Diffuse"),
    TEXT("Light2Specular"),
    TEXT("Light2Attenuation"),
    TEXT("Light2SpotCutoff"),

    TEXT("Light3Position"),
    TEXT("Light3Direction"),
    TEXT("Light3Ambient"),
    TEXT("Light3Diffuse"),
    TEXT("Light3Specular"),
    TEXT("Light3Attenuation"),
    TEXT("Light3SpotCutoff"),

    TEXT("Light4Position"),
    TEXT("Light4Direction"),
    TEXT("Light4Ambient"),
    TEXT("Light4Diffuse"),
    TEXT("Light4Specular"),
    TEXT("Light4Attenuation"),
    TEXT("Light4SpotCutoff"),

    TEXT("Light5Position"),
    TEXT("Light5Direction"),
    TEXT("Light5Ambient"),
    TEXT("Light5Diffuse"),
    TEXT("Light5Specular"),
    TEXT("Light5Attenuation"),
    TEXT("Light5SpotCutoff"),

    TEXT("Light6Position"),
    TEXT("Light6Direction"),
    TEXT("Light6Ambient"),
    TEXT("Light6Diffuse"),
    TEXT("Light6Specular"),
    TEXT("Light6Attenuation"),
    TEXT("Light6SpotCutoff"),

    TEXT("Light7Position"),
    TEXT("Light7Direction"),
    TEXT("Light7Ambient"),
    TEXT("Light7Diffuse"),
    TEXT("Light7Specular"),
    TEXT("Light7Attenuation"),
    TEXT("Light7SpotCutoff"),
};

RenderingConstantType ShaderRenderingConstant::GetType( const KChar * strName )
{
    for(UInt i = 0; i < RENDERING_CONST_COUNT; ++i) {
        if ( StrCmp( strName, sm_arrTypeToString[i] ) == 0 )
            return (RenderingConstantType)i;
    }
    return RENDERING_CONST_UNDEFINED;
}

ShaderRenderingConstant::ShaderRenderingConstant()
{
    m_iType = RENDERING_CONST_UNDEFINED;
    // uninitialized
}
ShaderRenderingConstant::ShaderRenderingConstant( RenderingConstantType iType, UInt iBaseRegister, UInt iRegisterCount )
{
    m_iType = iType;
    m_iBaseRegister = iBaseRegister;
    m_iRegisterCount = iRegisterCount;    
}
ShaderRenderingConstant::~ShaderRenderingConstant()
{
    // nothing to do
}

ShaderNumericConstant::ShaderNumericConstant()
{
    // uninitialized
}
ShaderNumericConstant::ShaderNumericConstant( UInt iRegister, Scalar arrValues[4] )
{
    m_iRegister = iRegister;
    m_arrData[0] = arrValues[0];
    m_arrData[1] = arrValues[1];
    m_arrData[2] = arrValues[2];
    m_arrData[3] = arrValues[3];
}
ShaderNumericConstant::~ShaderNumericConstant()
{
    // nothing to do
}

ShaderUserConstant::ShaderUserConstant()
{
    m_strName[0] = NULLBYTE;
    // uninitialized
}
ShaderUserConstant::ShaderUserConstant( const KChar * strName, UInt iBaseRegister, UInt iRegisterCount )
{
    StrCopy( m_strName, strName );
    m_iBaseRegister = iBaseRegister;
    m_iRegisterCount = iRegisterCount;
}
ShaderUserConstant::~ShaderUserConstant()
{
    // nothing to do
}

ShaderSamplerDescriptor::ShaderSamplerDescriptor()
{
    m_strName[0] = NULLBYTE;
    m_iDimension = 0;
    // uninitialized
}
ShaderSamplerDescriptor::ShaderSamplerDescriptor( const KChar * strName, SamplerType iType, UInt iTextureChannel )
{
    StrCopy( m_strName, strName );
    m_iType = iType;
    m_iTextureChannel = iTextureChannel;
    switch(m_iType) {
        case SAMPLER_1D: m_iDimension = 1; break;
        case SAMPLER_2D: m_iDimension = 2; break;
        case SAMPLER_3D: m_iDimension = 3; break;
        case SAMPLER_CUBE: m_iDimension = 2; break;
        case SAMPLER_PROJ: m_iDimension = 2; break;
        default: Assert(false); break;
    }
}
ShaderSamplerDescriptor::~ShaderSamplerDescriptor()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// ShaderProgram implementation
const KChar ShaderProgram::sm_Token_LineDelimiter = TEXT('\n');
const KChar ShaderProgram::sm_Token_Delimiter =     TEXT(' ');
const KChar ShaderProgram::sm_Token_Comment =       TEXT(';');
const KChar ShaderProgram::sm_Token_Declaration =   TEXT('#');
const KChar ShaderProgram::sm_Token_RegConstant =   TEXT('c');
const KChar ShaderProgram::sm_Token_IOConstant =    TEXT('&');

const KChar * ShaderProgram::sm_Declare_Const =    TEXT("const");
const KChar * ShaderProgram::sm_Declare_Var =      TEXT("var");

const KChar * ShaderProgram::sm_Type_Float =       TEXT("Float");
const KChar * ShaderProgram::sm_Type_Float1 =      TEXT("Float1");
const KChar * ShaderProgram::sm_Type_Float1x1 =    TEXT("Float1x1");
const KChar * ShaderProgram::sm_Type_Float1x2 =    TEXT("Float1x2");
const KChar * ShaderProgram::sm_Type_Float1x3 =    TEXT("Float1x3");
const KChar * ShaderProgram::sm_Type_Float1x4 =    TEXT("Float1x4");
const KChar * ShaderProgram::sm_Type_Float2 =      TEXT("Float2");
const KChar * ShaderProgram::sm_Type_Float2x1 =    TEXT("Float2x1");
const KChar * ShaderProgram::sm_Type_Float2x2 =    TEXT("Float2x2");
const KChar * ShaderProgram::sm_Type_Float2x3 =    TEXT("Float2x3");
const KChar * ShaderProgram::sm_Type_Float2x4 =    TEXT("Float2x4");
const KChar * ShaderProgram::sm_Type_Float3 =      TEXT("Float3");
const KChar * ShaderProgram::sm_Type_Float3x1 =    TEXT("Float3x1");
const KChar * ShaderProgram::sm_Type_Float3x2 =    TEXT("Float3x2");
const KChar * ShaderProgram::sm_Type_Float3x3 =    TEXT("Float3x3");
const KChar * ShaderProgram::sm_Type_Float3x4 =    TEXT("Float3x4");
const KChar * ShaderProgram::sm_Type_Float4 =      TEXT("Float4");
const KChar * ShaderProgram::sm_Type_Float4x1 =    TEXT("Float4x1");
const KChar * ShaderProgram::sm_Type_Float4x2 =    TEXT("Float4x2");
const KChar * ShaderProgram::sm_Type_Float4x3 =    TEXT("Float4x3");
const KChar * ShaderProgram::sm_Type_Float4x4 =    TEXT("Float4x4");
const KChar * ShaderProgram::sm_Type_Sampler1D =   TEXT("Sampler1D");
const KChar * ShaderProgram::sm_Type_Sampler2D =   TEXT("Sampler2D");
const KChar * ShaderProgram::sm_Type_Sampler3D =   TEXT("Sampler3D");
const KChar * ShaderProgram::sm_Type_SamplerCube = TEXT("SamplerCube");
const KChar * ShaderProgram::sm_Type_SamplerProj = TEXT("SamplerProj");

const KChar * ShaderProgram::sm_IOStatus_In =       TEXT("in");
const KChar * ShaderProgram::sm_IOStatus_Out =      TEXT("out");

const KChar * ShaderProgram::sm_Semantic_Position =     TEXT("POSITION");
const KChar * ShaderProgram::sm_Semantic_RasterPos =    TEXT("RASTERPOS");
const KChar * ShaderProgram::sm_Semantic_Depth =        TEXT("DEPTH");
const KChar * ShaderProgram::sm_Semantic_InvertW =      TEXT("INVERTW");
const KChar * ShaderProgram::sm_Semantic_RegularW =     TEXT("REGULARW");
const KChar * ShaderProgram::sm_Semantic_Normal =       TEXT("NORMAL");
const KChar * ShaderProgram::sm_Semantic_Tangent =      TEXT("TANGENT");
const KChar * ShaderProgram::sm_Semantic_BiNormal =     TEXT("BINORMAL");
const KChar * ShaderProgram::sm_Semantic_TexCoord =     TEXT("TEXCOORD");
const KChar * ShaderProgram::sm_Semantic_Color =        TEXT("COLOR");
const KChar * ShaderProgram::sm_Semantic_Diffuse =      TEXT("DIFFUSE");
const KChar * ShaderProgram::sm_Semantic_Specular =     TEXT("SPECULAR");
const KChar * ShaderProgram::sm_Semantic_BlendWeight =  TEXT("BLENDWEIGHT");
const KChar * ShaderProgram::sm_Semantic_BlendIndices = TEXT("BLENDINDICES");
const KChar * ShaderProgram::sm_Semantic_FogCoord =     TEXT("FOGCOORD");
const KChar * ShaderProgram::sm_Semantic_FogFactor =    TEXT("FOGFACTOR");
const KChar * ShaderProgram::sm_Semantic_Sample =       TEXT("SAMPLE");
const KChar * ShaderProgram::sm_Semantic_PointSize =    TEXT("POINTSIZE");
const KChar * ShaderProgram::sm_Semantic_UserData =     TEXT("USERDATA");

KChar ShaderProgram::sm_strVarName[SHADER_MAX_IDENT_SIZE] = TEXT("");
UInt ShaderProgram::sm_iFloatCount = 0;
UInt ShaderProgram::sm_iRegisterCount = 0;

ShaderProgram::ShaderProgram():
    m_arrRenderingConstants( INVALID_OFFSET, RenderFn->GetMemoryContext(), RenderFn->m_idAllocShaderHeap ),
    m_arrNumericConstants( INVALID_OFFSET, RenderFn->GetMemoryContext(), RenderFn->m_idAllocShaderHeap ),
    m_arrUserConstants( INVALID_OFFSET, RenderFn->GetMemoryContext(), RenderFn->m_idAllocShaderHeap ),
    m_arrSamplerDescriptors( INVALID_OFFSET, RenderFn->GetMemoryContext(), RenderFn->m_idAllocShaderHeap )
{
    m_strCodeText[0] = NULLBYTE;

    m_pInputCFL = RenderFn->CreateCFL();
    m_pOutputCFL = RenderFn->CreateCFL();

    m_bLoaded = false;
}
ShaderProgram::~ShaderProgram()
{
    RenderFn->DestroyCFL(m_pInputCFL);
    RenderFn->DestroyCFL(m_pOutputCFL);
}

ShaderRenderingConstant * ShaderProgram::GetRenderingConstant( RenderingConstantType iType ) const
{
    for(UInt i=0; i<m_arrRenderingConstants.Length(); ++i) {
        if ( m_arrRenderingConstants[i].GetType() == iType )
            return (ShaderRenderingConstant*)( &(m_arrRenderingConstants[i]) );
    }
    Assert(false);
    return NULL;
}
ShaderUserConstant * ShaderProgram::GetUserConstant( const KChar * strName ) const
{
    for(UInt i=0; i<m_arrUserConstants.Length(); ++i) {
        if ( StrCmp(m_arrUserConstants[i].GetName(), strName) == 0 )
            return (ShaderUserConstant*)( &(m_arrUserConstants[i]) );
    }
    Assert(false);
    return NULL;
}
ShaderSamplerDescriptor * ShaderProgram::GetSamplerDesc( const KChar * strName ) const
{
    for(UInt i=0; i<m_arrSamplerDescriptors.Length(); ++i) {
        if ( StrCmp(m_arrSamplerDescriptors[i].GetName(), strName) == 0 )
            return (ShaderSamplerDescriptor*)( &(m_arrSamplerDescriptors[i]) );
    }
    Assert(false);
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////

Bool ShaderProgram::_LoadFromText( const KChar * strProgramText, ShaderProgram * pOutProgram )
{
    Assert(pOutProgram != NULL);

    // Preprocess declarations & comment lines
    UInt iCodeLength = 0;
    const KChar * strLine = NULL;
    while( *strProgramText != NULLBYTE ) {
        // read line + delimiter
        strLine = strProgramText;
        while( *strProgramText != sm_Token_LineDelimiter )
            ++strProgramText;
        ++strProgramText;
        // skip comment or empty line
        if ( *strLine == sm_Token_Comment || *strLine == sm_Token_Delimiter )
            continue;
        if ( *strLine == sm_Token_Declaration ) {
            // declaration line
            if ( !_ParseDeclaration( strLine, pOutProgram ) )
                return false;
        } else {
            // code line
            Assert( (UInt)(strProgramText - strLine) <= (SHADER_MAX_CODE_SIZE - iCodeLength) );
            iCodeLength += StrCopyN( pOutProgram->m_strCodeText + iCodeLength, strLine, (strProgramText - strLine) );
        }
    }

    pOutProgram->m_pInputCFL->Finalize();
    pOutProgram->m_pOutputCFL->Finalize();
    return true;
}
Bool ShaderProgram::_LoadFromFile( const KChar * strFilename, ShaderProgram * pOutProgram )
{
    Assert(pOutProgram != NULL);

    File shaderFile;
    FileError fError = shaderFile.Open( strFilename, FILE_READ );
    Assert(fError == FILE_OK);

    KChar strProgramText[SHADER_MAX_CODE_SIZE];
    UInt iCodeLength = 0, iLineLength;
    while( !(shaderFile.EndOfFile()) ) {
        shaderFile.GetString( strProgramText + iCodeLength, SHADER_MAX_CODE_SIZE - iCodeLength,
                              &iLineLength, sm_Token_LineDelimiter, false );
        iCodeLength += iLineLength;
    }
    strProgramText[iCodeLength] = NULLBYTE;

    shaderFile.Close();

    return _LoadFromText( strProgramText, pOutProgram );
}

Bool ShaderProgram::_ParseDeclaration( const KChar * strLine, ShaderProgram * pOutProgram )
{
    // eat # and keyword
    const KChar * strToken = ++strLine;
    while(*strLine != sm_Token_Delimiter)
        ++strLine;
    // switch with declaration kind
    if ( StrCmpN( strToken, sm_Declare_Const, (strLine - strToken) ) == 0 ) {
        // eat spaces
        while(*strLine == sm_Token_Delimiter)
            ++strLine;
        return _ParseDeclaration_Const( strLine, pOutProgram );
    } else if ( StrCmpN( strToken, sm_Declare_Var, (strLine - strToken) ) == 0 ) {
        // eat spaces
        while(*strLine == sm_Token_Delimiter)
            ++strLine;
        return _ParseDeclaration_Var( strLine, pOutProgram );
    } else
        return false;
}
Bool ShaderProgram::_ParseDeclaration_Const( const KChar * strLine, ShaderProgram * pOutProgram )
{
    // "c[<index>] = <float0> <float1> <float2> <float3>"
    Scalar arrValues[4];
    KChar ch;

    // eat "c["
    ch = *strLine++;
    if ( ch != sm_Token_RegConstant )
        return false;
    ch = *strLine++;
    if ( ch != TEXT('[') )
        return false;
    // eat index and ']'
    UInt iBaseRegister = (UInt)( StrToUInt( strLine, &strLine ) );
    ch = *strLine++;
    if ( ch != TEXT(']') )
        return false;
    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // eat '='
    ch = *strLine++;
    if ( ch != TEXT('=') )
        return false;

    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // parse float0
    arrValues[0] = (Scalar)( StrToFloat( strLine, &strLine ) );
    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // parse float1
    arrValues[1] = (Scalar)( StrToFloat( strLine, &strLine ) );
    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // parse float2
    arrValues[2] = (Scalar)( StrToFloat( strLine, &strLine ) );
    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // parse float3
    arrValues[3] = (Scalar)( StrToFloat( strLine, &strLine ) );

    // accept
    pOutProgram->m_arrNumericConstants.Push( ShaderNumericConstant(iBaseRegister, arrValues) );
    return true;
}
Bool ShaderProgram::_ParseDeclaration_Var( const KChar * strLine, ShaderProgram * pOutProgram )
{
    // "<type> <texunit>? <name> <storage>?"
    SamplerType iSamplerType = SAMPLER_UNDEFINED;
    UInt iTexUnit = INVALID_OFFSET;

    // eat type
    const KChar * strType = strLine;
    while(*strLine != sm_Token_Delimiter)
        ++strLine;
    // switch with type
    if ( StrCmpN( strType, sm_Type_Float, (strLine - strType) ) == 0 )            { sm_iRegisterCount = 1; sm_iFloatCount = 1; }
    else if ( StrCmpN( strType, sm_Type_Float1, (strLine - strType) ) == 0 )      { sm_iRegisterCount = 1; sm_iFloatCount = 1; }
    else if ( StrCmpN( strType, sm_Type_Float1x1, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 1; sm_iFloatCount = 1; }
    else if ( StrCmpN( strType, sm_Type_Float1x2, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 1; sm_iFloatCount = 2; }
    else if ( StrCmpN( strType, sm_Type_Float1x3, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 1; sm_iFloatCount = 3; }
    else if ( StrCmpN( strType, sm_Type_Float1x4, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 1; sm_iFloatCount = 4; }
    else if ( StrCmpN( strType, sm_Type_Float2, (strLine - strType) ) == 0 )      { sm_iRegisterCount = 1; sm_iFloatCount = 2; }
    else if ( StrCmpN( strType, sm_Type_Float2x1, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 2; sm_iFloatCount = 2; }
    else if ( StrCmpN( strType, sm_Type_Float2x2, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 2; sm_iFloatCount = 4; }
    else if ( StrCmpN( strType, sm_Type_Float2x3, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 2; sm_iFloatCount = 6; }
    else if ( StrCmpN( strType, sm_Type_Float2x4, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 2; sm_iFloatCount = 8; }
    else if ( StrCmpN( strType, sm_Type_Float3, (strLine - strType) ) == 0 )      { sm_iRegisterCount = 1; sm_iFloatCount = 3; }
    else if ( StrCmpN( strType, sm_Type_Float3x1, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 3; sm_iFloatCount = 3; }
    else if ( StrCmpN( strType, sm_Type_Float3x2, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 3; sm_iFloatCount = 6; }
    else if ( StrCmpN( strType, sm_Type_Float3x3, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 3; sm_iFloatCount = 9; }
    else if ( StrCmpN( strType, sm_Type_Float3x4, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 3; sm_iFloatCount = 12; }
    else if ( StrCmpN( strType, sm_Type_Float4, (strLine - strType) ) == 0 )      { sm_iRegisterCount = 1; sm_iFloatCount = 4; }
    else if ( StrCmpN( strType, sm_Type_Float4x1, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 4; sm_iFloatCount = 4; }
    else if ( StrCmpN( strType, sm_Type_Float4x2, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 4; sm_iFloatCount = 8; }
    else if ( StrCmpN( strType, sm_Type_Float4x3, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 4; sm_iFloatCount = 12; }
    else if ( StrCmpN( strType, sm_Type_Float4x4, (strLine - strType) ) == 0 )    { sm_iRegisterCount = 4; sm_iFloatCount = 16; }
    else if ( StrCmpN( strType, sm_Type_Sampler1D, (strLine - strType) ) == 0 )   { iSamplerType = SAMPLER_1D; }
    else if ( StrCmpN( strType, sm_Type_Sampler2D, (strLine - strType) ) == 0 )   { iSamplerType = SAMPLER_2D; }
    else if ( StrCmpN( strType, sm_Type_Sampler3D, (strLine - strType) ) == 0 )   { iSamplerType = SAMPLER_3D; }
    else if ( StrCmpN( strType, sm_Type_SamplerCube, (strLine - strType) ) == 0 ) { iSamplerType = SAMPLER_CUBE; }
    else if ( StrCmpN( strType, sm_Type_SamplerProj, (strLine - strType) ) == 0 ) { iSamplerType = SAMPLER_PROJ; }
    else
        return false;

    if (iSamplerType != SAMPLER_UNDEFINED) {
        // eat spaces
        while(*strLine == sm_Token_Delimiter)
            ++strLine;
        // eat texunit index
        iTexUnit = (UInt)( StrToUInt( strLine, &strLine ) );
        if (iTexUnit >= SHADER_MAX_SAMPLERS)
            return false;
    }

    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // eat name
    const KChar * strVarName = strLine;
    while(*strLine != sm_Token_Delimiter && *strLine != sm_Token_LineDelimiter)
        ++strLine;
    UInt iNameLength = (strLine - strVarName);
    Assert(iNameLength < SHADER_MAX_IDENT_SIZE);
    StrCopyN( sm_strVarName, strVarName, iNameLength + 1 );

    if (iSamplerType != SAMPLER_UNDEFINED) {
        // accept
        pOutProgram->m_arrSamplerDescriptors.Push(
            ShaderSamplerDescriptor( sm_strVarName, iSamplerType, iTexUnit )
        );
        return true;
    }

    // eat spaces
    while(*strLine == sm_Token_Delimiter)
        ++strLine;
    // switch with storage kind
    if ( *strLine == sm_Token_IOConstant )
        return _ParseDeclaration_Var_IO( strLine, pOutProgram );
    else if ( *strLine == sm_Token_RegConstant )
        return _ParseDeclaration_Var_Reg( strLine, pOutProgram );
    else
        return false;
}
Bool ShaderProgram::_ParseDeclaration_Var_IO( const KChar * strLine, ShaderProgram * pOutProgram )
{
    // <storage> = "&(in|out).<semantic>"

    // eat in/out flag and '.'
    const KChar * strIO = ++strLine;
    while( *strLine != TEXT('.') )
        ++strLine;
    ++strLine;

    // eat semantic
    const KChar * strSemantic = strLine;
    while(*strLine != sm_Token_Delimiter && *strLine != sm_Token_LineDelimiter)
        ++strLine;

    // switch with IO flag
    if ( StrCmpN( sm_IOStatus_In, strIO, 2 ) == 0 ) {
        // switch with semantic
        if ( StrCmpN( strSemantic, sm_Semantic_Position, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VERTEX3, CFLSEMANTIC_POSITION );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VERTEX4, CFLSEMANTIC_POSITION );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_RasterPos, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VERTEX2, CFLSEMANTIC_RASTERPOS );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Depth, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_DEPTH );
        } else if ( StrCmpN( strSemantic, sm_Semantic_InvertW, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_INVERTW );
        } else if ( StrCmpN( strSemantic, sm_Semantic_RegularW, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_REGULARW );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Normal, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR3, CFLSEMANTIC_NORMAL );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_NORMAL );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_Tangent, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR3, CFLSEMANTIC_TANGENT );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_TANGENT );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_BiNormal, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR3, CFLSEMANTIC_BINORMAL );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_BINORMAL );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_TexCoord, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 1)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_TEXCOORD1, CFLSEMANTIC_TEXCOORD );
            else if (sm_iFloatCount == 2)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_TEXCOORD2, CFLSEMANTIC_TEXCOORD );
            else if (sm_iFloatCount == 3)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_TEXCOORD3, CFLSEMANTIC_TEXCOORD );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_TEXCOORD4, CFLSEMANTIC_TEXCOORD );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_Color, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_COLOR, CFLSEMANTIC_COLOR );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Diffuse, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_COLOR, CFLSEMANTIC_DIFFUSE );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Specular, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_COLOR, CFLSEMANTIC_SPECULAR );
        } else if ( StrCmpN( strSemantic, sm_Semantic_BlendWeight, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_BLENDWEIGHT );
        } else if ( StrCmpN( strSemantic, sm_Semantic_BlendIndices, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_BLENDINDICES );
        } else if ( StrCmpN( strSemantic, sm_Semantic_FogCoord, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VERTEX4, CFLSEMANTIC_FOGCOORD );
        } else if ( StrCmpN( strSemantic, sm_Semantic_FogFactor, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_FOGFACTOR );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Sample, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_SAMPLE );
        } else if ( StrCmpN( strSemantic, sm_Semantic_PointSize, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_POINT2, CFLSEMANTIC_POINTSIZE );
        } else if ( StrCmpN( strSemantic, sm_Semantic_UserData, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pInputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_USERDATA );
        } else
            return false;
    } else if ( StrCmpN( sm_IOStatus_Out, strIO, 3 ) == 0 ) {
        // switch with semantic
        if ( StrCmpN( strSemantic, sm_Semantic_Position, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VERTEX3, CFLSEMANTIC_POSITION );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VERTEX4, CFLSEMANTIC_POSITION );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_RasterPos, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VERTEX2, CFLSEMANTIC_RASTERPOS );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Depth, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_DEPTH );
        } else if ( StrCmpN( strSemantic, sm_Semantic_InvertW, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_INVERTW );
        } else if ( StrCmpN( strSemantic, sm_Semantic_RegularW, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_FLOAT, CFLSEMANTIC_REGULARW );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Normal, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR3, CFLSEMANTIC_NORMAL );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_NORMAL );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_Tangent, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR3, CFLSEMANTIC_TANGENT );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_TANGENT );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_BiNormal, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 3)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR3, CFLSEMANTIC_BINORMAL );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_BINORMAL );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_TexCoord, (strLine - strSemantic) ) == 0 ) {
            if (sm_iFloatCount == 1)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_TEXCOORD1, CFLSEMANTIC_TEXCOORD );
            else if (sm_iFloatCount == 2)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_TEXCOORD2, CFLSEMANTIC_TEXCOORD );
            else if (sm_iFloatCount == 3)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_TEXCOORD3, CFLSEMANTIC_TEXCOORD );
            else if (sm_iFloatCount == 4)
                pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_TEXCOORD4, CFLSEMANTIC_TEXCOORD );
            else
                return false;
        } else if ( StrCmpN( strSemantic, sm_Semantic_Color, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_COLOR, CFLSEMANTIC_COLOR );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Diffuse, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_COLOR, CFLSEMANTIC_DIFFUSE );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Specular, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_COLOR, CFLSEMANTIC_SPECULAR );
        } else if ( StrCmpN( strSemantic, sm_Semantic_BlendWeight, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_BLENDWEIGHT );
        } else if ( StrCmpN( strSemantic, sm_Semantic_BlendIndices, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_BLENDINDICES );
        } else if ( StrCmpN( strSemantic, sm_Semantic_FogCoord, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VERTEX4, CFLSEMANTIC_FOGCOORD );
        } else if ( StrCmpN( strSemantic, sm_Semantic_FogFactor, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_FOGFACTOR );
        } else if ( StrCmpN( strSemantic, sm_Semantic_Sample, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_SAMPLE );
        } else if ( StrCmpN( strSemantic, sm_Semantic_PointSize, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_POINT2, CFLSEMANTIC_POINTSIZE );
        } else if ( StrCmpN( strSemantic, sm_Semantic_UserData, (strLine - strSemantic) ) == 0 ) {
            pOutProgram->m_pOutputCFL->DeclareField( CFLTYPE_VECTOR4, CFLSEMANTIC_USERDATA );
        } else
            return false;
    } else
        return false;

    // accept
    return true;
}
Bool ShaderProgram::_ParseDeclaration_Var_Reg( const KChar * strLine, ShaderProgram * pOutProgram )
{
    // <storage> = "c[<index>]"
    KChar ch;

    // eat "c["
    ++strLine;
    ch = *strLine++;
    if ( ch != TEXT('[') )
        return false;
    // eat index and ']'
    UInt iBaseRegister = (UInt)( StrToUInt( strLine, &strLine ) );
    ch = *strLine++;
    if ( ch != TEXT(']') )
        return false;

    // accept
    RenderingConstantType iRCType = ShaderRenderingConstant::GetType(sm_strVarName);
    if (iRCType != RENDERING_CONST_UNDEFINED)
        pOutProgram->m_arrRenderingConstants.Push( ShaderRenderingConstant(iRCType, iBaseRegister, sm_iRegisterCount) );
    else
        pOutProgram->m_arrUserConstants.Push( ShaderUserConstant(sm_strVarName, iBaseRegister, sm_iRegisterCount) );
    return true;
}

Void ShaderProgram::_Prepare( Scalar * arrRegisters, Sampler ** arrSamplers ) const
{
    // Load Constants
    UInt i, j;
    Scalar * pReg;
    for(i = 0; i < m_arrRenderingConstants.Length(); ++i) {
        pReg = arrRegisters + ( m_arrRenderingConstants[i].GetBaseRegister() * SHADER_REGISTER_CHANNELS );
        RenderFn->_GetShaderConstant( pReg, m_arrRenderingConstants[i].GetType() );
    }
    for(i = 0; i < m_arrNumericConstants.Length(); ++i) {
        pReg = arrRegisters + ( m_arrNumericConstants[i].GetRegister() * SHADER_REGISTER_CHANNELS );
        for(j = 0; j < SHADER_REGISTER_CHANNELS; ++j)
            pReg[j] = m_arrNumericConstants[i].GetData()[j];
    }
    for(i = 0; i < m_arrUserConstants.Length(); ++i) {
        pReg = arrRegisters + ( m_arrUserConstants[i].GetBaseRegister() * SHADER_REGISTER_CHANNELS );
        RenderFn->_GetShaderUserConstant( pReg, m_arrUserConstants[i].GetRegisterCount() * SHADER_REGISTER_CHANNELS,
                                           m_arrUserConstants[i].GetName() );
    }
    if (arrSamplers != NULL) {
        for(i = 0; i < m_arrSamplerDescriptors.Length(); ++i) {
            arrSamplers[ m_arrSamplerDescriptors[i].GetTextureChannel() ] =
                RenderFn->_GetSampler( m_arrSamplerDescriptors[i].GetTextureChannel() );
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////
// VertexShaderProgram implementation
VertexShaderProgram::VertexShaderProgram( const KChar * strLoadString, Bool bLoadFromFile ):
    ShaderProgram()
{
    if (bLoadFromFile)
        m_bLoaded = ShaderProgram::_LoadFromFile( strLoadString, this );
    else
        m_bLoaded = ShaderProgram::_LoadFromText( strLoadString, this );
    Assert(m_bLoaded);
}
VertexShaderProgram::~VertexShaderProgram()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// PixelShaderProgram implementation
PixelShaderProgram::PixelShaderProgram( const KChar * strLoadString, Bool bLoadFromFile ):
    ShaderProgram()
{
    if (bLoadFromFile)
        m_bLoaded = ShaderProgram::_LoadFromFile( strLoadString, this );
    else
        m_bLoaded = ShaderProgram::_LoadFromText( strLoadString, this );
    Assert(m_bLoaded);
}
PixelShaderProgram::~PixelShaderProgram()
{
    // nothing to do
}

