<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/CodeDoc/CodeDocGenerator.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : CodeDoc database generator (using DOxygen's XML output)
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
	// Documentation paths
define( "CODEDOC_XML_ROOTPATH", "D:/Programming/_Projects/Scarab Engine/Documentation" );

	// DOxygen definitions
define( "CODEDOC_XML_INDEX",    "index" );
define( "CODEDOC_XML_VERSION",  "1.8.3.1" ); // DOxygen's output version

define( "CODEDOC_XML_COMPOUNDKIND_DIR",       "dir" );
define( "CODEDOC_XML_COMPOUNDKIND_FILE",      "file" );
define( "CODEDOC_XML_COMPOUNDKIND_NAMESPACE", "namespace" );
define( "CODEDOC_XML_COMPOUNDKIND_UNION",     "union" );
define( "CODEDOC_XML_COMPOUNDKIND_STRUCT",    "struct" );
define( "CODEDOC_XML_COMPOUNDKIND_CLASS",     "class" );
define( "CODEDOC_XML_COMPOUNDKIND_INTERFACE", "interface" );

define( "CODEDOC_XML_SECTIONKIND_DEFINE",	"define" );
define( "CODEDOC_XML_SECTIONKIND_ENUM",		"enum" );
define( "CODEDOC_XML_SECTIONKIND_TYPEDEF",	"typedef" );
define( "CODEDOC_XML_SECTIONKIND_VARIABLE", "var" );
define( "CODEDOC_XML_SECTIONKIND_FUNCTION", "func" );

define( "CODEDOC_XML_MEMBERKIND_DEFINE",    "define" );
define( "CODEDOC_XML_MEMBERKIND_ENUM",      "enum" );
define( "CODEDOC_XML_MEMBERKIND_TYPEDEF",   "typedef" );
define( "CODEDOC_XML_MEMBERKIND_VARIABLE",  "variable" );
define( "CODEDOC_XML_MEMBERKIND_FUNCTION",  "function" );

define( "CODEDOC_XML_PROTECTION_PUBLIC",    "public" );
define( "CODEDOC_XML_PROTECTION_PROTECTED", "protected" );
define( "CODEDOC_XML_PROTECTION_PRIVATE",   "private" );

define( "CODEDOC_XML_VIRTUAL_NO",   "non-virtual" );
define( "CODEDOC_XML_VIRTUAL_YES",  "virtual" );
define( "CODEDOC_XML_VIRTUAL_PURE", "pure-virtual" );

	// Temp value
define( "CODEDOC_SAMPLE_SHORT_DESC",  "Coming soon (ShortDesc) ..." );
define( "CODEDOC_SAMPLE_DOC_CONTENT", "Coming soon (DocContent) ..." );

/////////////////////////////////////////////////////////////////////////////////
// The DOxygenIndex class
class DOxygenIndex
{
	// Constructor / Destructor
    public function __construct()
    {
		$hXML = XMLManager::GetSingleton();

		$hDocument = $hXML->CreateDocument();
		$bOk = $hDocument->LoadFromFile( CODEDOC_XML_ROOTPATH . "/xml/" . CODEDOC_XML_INDEX . ".xml" );
		assert( '$bOk' );
		$this->m_hRoot = $hDocument->GetRoot();

		$this->m_arrCompounds = $this->m_hRoot->GetChildrenByTag( "compound" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetVersion() {
		assert( '$this->m_hRoot->HasAttribute("version")' );
		return $this->m_hRoot->GetAttributeValue( "version" );
	}

	// Children
	public function GetCompoundDeclCount() {
		return count( $this->m_arrCompounds );
	}
	public function GetCompoundDecl( $i ) {
		return new DoxyCompoundDecl( $this->m_arrCompounds[$i] );
	}

	// Members
	private $m_hRoot;
	private $m_arrCompounds;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyCompoundDecl class
class DoxyCompoundDecl
{
	// Constructor / Destructor
    public function __construct( $hCompound )
    {
		assert( '$hCompound->GetTagName() == "compound"' );
		$this->m_hCompound = $hCompound;

		$this->m_hName = $this->m_hCompound->GetChildByTag( "name" );
		assert( '$this->m_hName != NULL' );

		$this->m_arrMembers = $this->m_hCompound->GetChildrenByTag( "member" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetRefID() {
		assert( '$this->m_hCompound->HasAttribute("refid")' );
		return $this->m_hCompound->GetAttributeValue( "refid" );
	}
	public function GetKind() {
		assert( '$this->m_hCompound->HasAttribute("kind")' );
		return $this->m_hCompound->GetAttributeValue( "kind" );
	}

	// Children
	public function GetName() {
		return $this->m_hName->GetTextContent();
	}
	
	public function GetMemberDeclCount() {
		return count( $this->m_arrMembers );
	}
	public function GetMemberDecl( $i ) {
		return new DoxyMemberDecl( $this->m_arrMembers[$i] );
	}

	// Members
	private $m_hCompound;
	private $m_hName;
	private $m_arrMembers;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyMemberDecl class
class DoxyMemberDecl
{
	// Constructor / Destructor
    public function __construct( $hMember )
    {
		assert( '$hMember->GetTagName() == "member"' );
		$this->m_hMember = $hMember;

		$this->m_hName = $this->m_hMember->GetChildByTag( "name" );
		assert( '$this->m_hName != NULL' );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetRefID() {
		assert( '$this->m_hMember->HasAttribute("refid")' );
		return $this->m_hMember->GetAttributeValue( "refid" );
	}
	public function GetKind() {
		assert( '$this->m_hMember->HasAttribute("kind")' );
		return $this->m_hMember->GetAttributeValue( "kind" );
	}

	// Children
	public function GetName() {
		return $this->m_hName->GetTextContent();
	}

	// Members
	private $m_hMember;
	private $m_hName;
}
		
/////////////////////////////////////////////////////////////////////////////////
// The DOxygen class
class DOxygen
{
	// Constructor / Destructor
    public function __construct( $strPathXML )
    {
		$hXML = XMLManager::GetSingleton();

		$hDocument = $hXML->CreateDocument();
		$bOk = $hDocument->LoadFromFile( CODEDOC_XML_ROOTPATH . "/xml/" . $strPathXML . ".xml" );
		assert( '$bOk' );
		$this->m_hRoot = $hDocument->GetRoot();

		$this->m_arrCompoundDefs = $this->m_hRoot->GetChildrenByTag( "compounddef" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetVersion() {
		assert( '$this->m_hRoot->HasAttribute("version")' );
		return $this->m_hRoot->GetAttributeValue( "version" );
	}

	// Children
	public function GetCompoundDefCount() {
		return count( $this->m_arrCompoundDefs );
	}
	public function GetCompoundDef( $i ) {
		return new DoxyCompoundDef( $this->m_arrCompoundDefs[$i] );
	}

	// Members
	private $m_hRoot;
	private $m_arrCompoundDefs;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyCompoundDef class
class DoxyCompoundDef
{
	// Constructor / Destructor
    public function __construct( $hCompoundDef )
    {
		assert( '$hCompoundDef->GetTagName() == "compounddef"' );
		$this->m_hCompoundDef = $hCompoundDef;

		$this->m_hName = $this->m_hCompoundDef->GetChildByTag( "compoundname" );
		assert( '$this->m_hName != NULL' );
		$this->m_hLocation = $this->m_hCompoundDef->GetChildByTag( "location" );
		assert( '$this->m_hLocation != NULL' );

		$this->m_arrIncludes = $this->m_hCompoundDef->GetChildrenByTag( "includes" );
		$this->m_arrIncludedby = $this->m_hCompoundDef->GetChildrenByTag( "includedby" );
		$this->m_arrInnerDirs = $this->m_hCompoundDef->GetChildrenByTag( "innerdir" );
		$this->m_arrInnerFiles = $this->m_hCompoundDef->GetChildrenByTag( "innerfile" );
		$this->m_arrInnerNamespaces = $this->m_hCompoundDef->GetChildrenByTag( "innernamespace" );
		$this->m_arrInnerTypes = $this->m_hCompoundDef->GetChildrenByTag( "innerclass" );
		$this->m_arrBaseTypes = $this->m_hCompoundDef->GetChildrenByTag( "basecompoundref" );
		$this->m_arrDerivedTypes = $this->m_hCompoundDef->GetChildrenByTag( "derivedcompoundref" );

		$this->m_arrTemplateParams = array();
		$hTemplateParamList = $this->m_hCompoundDef->GetChildByTag( "templateparamlist" );
		if ( $hTemplateParamList != NULL )
			$this->m_arrTemplateParams = $hTemplateParamList->GetChildrenByTag( "param" );

		$this->m_arrSectionDefs = $this->m_hCompoundDef->GetChildrenByTag( "sectiondef" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetID() {
		assert( '$this->m_hCompoundDef->HasAttribute("id")' );
		return $this->m_hCompoundDef->GetAttributeValue( "id" );
	}
	public function GetKind() {
		assert( '$this->m_hCompoundDef->HasAttribute("kind")' );
		return $this->m_hCompoundDef->GetAttributeValue( "kind" );
	}

	public function GetProtection() {
		if ( $this->m_hCompoundDef->HasAttribute("prot") )
			return $this->m_hCompoundDef->GetAttributeValue( "prot" );
		return CODEDOC_XML_PROTECTION_PUBLIC;
	}

	// Children
	public function GetName() {
		return $this->m_hName->GetTextContent();
	}
	public function GetLocationFile() {
		assert( '$this->m_hLocation->HasAttribute("file")' );
		return $this->m_hLocation->GetAttributeValue( "file" );
	}
	public function GetLocationLine() {
		if ( $this->m_hLocation->HasAttribute("line") )
			return intval( $this->m_hLocation->GetAttributeValue( "line" ) );
		return 0;
	}
	public function GetLocationBodyFile() {
		if ( $this->m_hLocation->HasAttribute("bodyfile") )
			return $this->m_hLocation->GetAttributeValue( "bodyfile" );
		return "";
	}
	public function GetLocationBodyStart() {
		if ( $this->m_hLocation->HasAttribute("bodystart") )
			return intval( $this->m_hLocation->GetAttributeValue( "bodystart" ) );
		return 0;
	}
	public function GetLocationBodyEnd() {
		if ( $this->m_hLocation->HasAttribute("bodyend") )
			return intval( $this->m_hLocation->GetAttributeValue( "bodyend" ) );
		return 0;
	}

	public function GetIncludesCount() {
		return count( $this->m_arrIncludes );
	}
	public function GetIncludes( $i ) {
		$hNode = $this->m_arrIncludes[$i];
		assert( '$hNode->HasAttribute("refid")' );
		assert( '$hNode->HasAttribute("local")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "local" => $hNode->GetAttributeValue( "local" ),
					  "name" => $hNode->GetTextContent() );
	}
	public function GetIncludedbyCount() {
		return count( $this->m_arrIncludedby );
	}
	public function GetIncludedby( $i ) {
		$hNode = $this->m_arrIncludedby[$i];
		assert( '$hNode->HasAttribute("refid")' );
		assert( '$hNode->HasAttribute("local")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "local" => $hNode->GetAttributeValue( "local" ),
					  "name" => $hNode->GetTextContent() );
	}

	public function GetInnerDirCount() {
		return count( $this->m_arrInnerDirs );
	}
	public function GetInnerDir( $i ) {
		$hNode = $this->m_arrInnerDirs[$i];
		assert( '$hNode->HasAttribute("refid")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "name" => $hNode->GetTextContent() );
	}
	public function GetInnerFileCount() {
		return count( $this->m_arrInnerFiles );
	}
	public function GetInnerFile( $i ) {
		$hNode = $this->m_arrInnerFiles[$i];
		assert( '$hNode->HasAttribute("refid")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "name" => $hNode->GetTextContent() );
	}
	public function GetInnerNamespaceCount() {
		return count( $this->m_arrInnerNamespaces );
	}
	public function GetInnerNamespace( $i ) {
		$hNode = $this->m_arrInnerNamespaces[$i];
		assert( '$hNode->HasAttribute("refid")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "name" => $hNode->GetTextContent() );
	}
	public function GetInnerTypeCount() {
		return count( $this->m_arrInnerTypes );
	}
	public function GetInnerType( $i ) {
		$hNode = $this->m_arrInnerTypes[$i];
		assert( '$hNode->HasAttribute("refid")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "name" => $hNode->GetTextContent() );
	}

	public function GetBaseTypeCount() {
		return count( $this->m_arrBaseTypes );
	}
	public function GetBaseType( $i ) {
		$hNode = $this->m_arrBaseTypes[$i];
		if ( !($hNode->HasAttribute("refid")) )
			print_r( $hNode );
		//assert( '$hNode->HasAttribute("refid")' );
		assert( '$hNode->HasAttribute("prot")' );
		assert( '$hNode->HasAttribute("virt")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "prot" => $hNode->GetAttributeValue( "prot" ),
					  "virtual" => $hNode->GetAttributeValue( "virt" ),
					  "name" => $hNode->GetTextContent() );
	}
	public function GetDerivedTypeCount() {
		return count( $this->m_arrDerivedTypes );
	}
	public function GetDerivedType( $i ) {
		$hNode = $this->m_arrDerivedTypes[$i];
		assert( '$hNode->HasAttribute("refid")' );
		assert( '$hNode->HasAttribute("prot")' );
		assert( '$hNode->HasAttribute("virt")' );
		return array( "refid" => $hNode->GetAttributeValue( "refid" ),
					  "prot" => $hNode->GetAttributeValue( "prot" ),
					  "virtual" => $hNode->GetAttributeValue( "virt" ),
					  "name" => $hNode->GetTextContent() );
	}

	public function GetTemplateParamCount() {
		return count( $this->m_arrTemplateParams );
	}
	public function GetTemplateParam( $i ) {
		return new DoxyParam( $this->m_arrTemplateParams[$i] );
	}

	public function GetSectionDefCount() {
		return count( $this->m_arrSectionDefs );
	}
	public function GetSectionDef( $i ) {
		return new DoxySectionDef( $this->m_arrSectionDefs[$i] );
	}

	// Members
	private $m_hCompoundDef;
	private $m_hName;
	private $m_hLocation;
	private $m_arrIncludes;
	private $m_arrIncludedby;
	private $m_arrInnerDirs;
	private $m_arrInnerFiles;
	private $m_arrInnerNamespaces;
	private $m_arrInnerTypes;
	private $m_arrBaseTypes;
	private $m_arrDerivedTypes;
	private $m_arrTemplateParams;
	private $m_arrSectionDefs;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxySectionDef class
class DoxySectionDef
{
	// Constructor / Destructor
    public function __construct( $hSectionDef )
    {
		assert( '$hSectionDef->GetTagName() == "sectiondef"' );
		$this->m_hSectionDef = $hSectionDef;

		$this->m_arrMemberDefs = $this->m_hSectionDef->GetChildrenByTag( "memberdef" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetKind() {
		assert( '$this->m_hSectionDef->HasAttribute("kind")' );
		return $this->m_hSectionDef->GetAttributeValue( "kind" );
	}

	// Children
	public function GetMemberDefCount() {
		return count( $this->m_arrMemberDefs );
	}
	public function GetMemberDef( $i ) {
		return new DoxyMemberDef( $this->m_arrMemberDefs[$i] );
	}

	// Members
	private $m_hSectionDef;
	private $m_arrMemberDefs;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyMemberDef class
class DoxyMemberDef
{
	// Constructor / Destructor
    public function __construct( $hMemberDef )
    {
		assert( '$hMemberDef->GetTagName() == "memberdef"' );
		$this->m_hMemberDef = $hMemberDef;
		
		$this->m_hName = $this->m_hMemberDef->GetChildByTag( "name" );
		assert( '$this->m_hName != NULL' );
		$this->m_hLocation = $this->m_hMemberDef->GetChildByTag( "location" );
		assert( '$this->m_hLocation != NULL' );
		$this->m_hDefinition = $this->m_hMemberDef->GetChildByTag( "definition" );
		$this->m_hArgString = $this->m_hMemberDef->GetChildByTag( "argsstring" );
		$this->m_hInitializer = $this->m_hMemberDef->GetChildByTag( "initializer" );

		$this->m_arrTemplateParams = array();
		$hTemplateParamList = $this->m_hMemberDef->GetChildByTag( "templateparamlist" );
		if ( $hTemplateParamList != NULL )
			$this->m_arrTemplateParams = $hTemplateParamList->GetChildrenByTag( "param" );

		$this->m_hType = $this->m_hMemberDef->GetChildByTag( "type" );
		$this->m_arrParams = $this->m_hMemberDef->GetChildrenByTag( "param" );
		$this->m_arrEnumValues = $this->m_hMemberDef->GetChildrenByTag( "enumvalue" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetID() {
		assert( '$this->m_hMemberDef->HasAttribute("id")' );
		return $this->m_hMemberDef->GetAttributeValue( "id" );
	}
	public function GetKind() {
		assert( '$this->m_hMemberDef->HasAttribute("kind")' );
		return $this->m_hMemberDef->GetAttributeValue( "kind" );
	}

	public function GetProtection() {
		if ( $this->m_hMemberDef->HasAttribute("prot") )
			return $this->m_hMemberDef->GetAttributeValue( "prot" );
		return CODEDOC_XML_PROTECTION_PUBLIC;
	}
	public function GetVirtual() {
		if ( $this->m_hMemberDef->HasAttribute("virt") )
			return $this->m_hMemberDef->GetAttributeValue( "virt" );
		return CODEDOC_XML_VIRTUAL_NO;
	}
	public function IsStatic() {
		if ( $this->m_hMemberDef->HasAttribute("static") )
			return ( $this->m_hMemberDef->GetAttributeValue("static") == "yes" );
		return false;
	}
	public function IsConst() {
		if ( $this->m_hMemberDef->HasAttribute("const") )
			return ( $this->m_hMemberDef->GetAttributeValue("const") == "yes" );
		return false;
	}
	public function IsVolatile() {
		if ( $this->m_hMemberDef->HasAttribute("volatile") )
			return ( $this->m_hMemberDef->GetAttributeValue("volatile") == "yes" );
		return false;
	}
	public function IsInline() {
		if ( $this->m_hMemberDef->HasAttribute("inline") )
			return ( $this->m_hMemberDef->GetAttributeValue("inline") == "yes" );
		return false;
	}
	public function IsExplicit() {
		if ( $this->m_hMemberDef->HasAttribute("explicit") )
			return ( $this->m_hMemberDef->GetAttributeValue("explicit") == "yes" );
		return false;
	}
	public function IsMutable() {
		if ( $this->m_hMemberDef->HasAttribute("mutable") )
			return ( $this->m_hMemberDef->GetAttributeValue("mutable") == "yes" );
		return false;
	}

	// Children
	public function GetName() {
		return $this->m_hName->GetTextContent();
	}
	public function GetLocationFile() {
		assert( '$this->m_hLocation->HasAttribute("file")' );
		return $this->m_hLocation->GetAttributeValue( "file" );
	}
	public function GetLocationLine() {
		if ( $this->m_hLocation->HasAttribute("line") )
			return intval( $this->m_hLocation->GetAttributeValue( "line" ) );
		return 0;
	}
	public function GetLocationBodyFile() {
		if ( $this->m_hLocation->HasAttribute("bodyfile") )
			return $this->m_hLocation->GetAttributeValue( "bodyfile" );
		return "";
	}
	public function GetLocationBodyStart() {
		if ( $this->m_hLocation->HasAttribute("bodystart") )
			return intval( $this->m_hLocation->GetAttributeValue( "bodystart" ) );
		return 0;
	}
	public function GetLocationBodyEnd() {
		if ( $this->m_hLocation->HasAttribute("bodyend") )
			return intval( $this->m_hLocation->GetAttributeValue( "bodyend" ) );
		return 0;
	}

	public function GetDefinition() {
		if ( $this->m_hDefinition != NULL )
			return $this->m_hDefinition->GetTextContent();
		return "";
	}
	public function GetArgString() {
		if ( $this->m_hArgString != NULL )
			return $this->m_hArgString->GetTextContent();
		return "";
	}

	public function GetType() {
		if ( $this->m_hType != NULL )
			return new DoxyLink( $this->m_hType );
		return NULL;
	}
	public function GetInitializer() {
		if ( $this->m_hInitializer != NULL )
			return new DoxyLink( $this->m_hInitializer );
		return NULL;
	}

	public function GetTemplateParamCount() {
		return count( $this->m_arrTemplateParams );
	}
	public function GetTemplateParam( $i ) {
		return new DoxyParam( $this->m_arrTemplateParams[$i] );
	}
	public function GetParamCount() {
		return count( $this->m_arrParams );
	}
	public function GetParam( $i ) {
		return new DoxyParam( $this->m_arrParams[$i] );
	}
	public function GetEnumValueCount() {
		return count( $this->m_arrEnumValues );
	}
	public function GetEnumValue( $i ) {
		return new DoxyEnumValue( $this->m_arrEnumValues[$i] );
	}

	// Members
	private $m_hMemberDef;
	private $m_hName;
	private $m_hLocation;
	private $m_hDefinition;
	private $m_hArgString;
	private $m_hInitializer;
	private $m_arrTemplateParams;
	private $m_hType;
	private $m_arrParams;
	private $m_arrEnumValues;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyParam class
class DoxyParam
{
	// Constructor / Destructor
    public function __construct( $hParam )
    {
		assert( '$hParam->GetTagName() == "param"' );
		$this->m_hParam = $hParam;

		$this->m_hType = $this->m_hParam->GetChildByTag( "type" );
		$this->m_hDeclName = $this->m_hParam->GetChildByTag( "declname" );
		$this->m_hDefName = $this->m_hParam->GetChildByTag( "defname" );
		$this->m_hArray = $this->m_hParam->GetChildByTag( "array" );
		$this->m_hDefValue = $this->m_hParam->GetChildByTag( "defval" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Children
	public function GetType() {
		if ( $this->m_hType != NULL )
			return new DoxyLink( $this->m_hType );
		return NULL;
	}
	public function GetDeclarationName() {
		if ( $this->m_hDeclName != NULL )
			return $this->m_hDeclName->GetTextContent();
		return "";
	}
	public function GetDefinitionName() {
		if ( $this->m_hDefName != NULL )
			return $this->m_hDefName->GetTextContent();
		return "";
	}
	public function IsArray() {
		if ( $this->m_hArray != NULL )
			return ( $this->m_hArray->GetTextContent() == "yes" );
		return false;
	}
	public function GetDefaultValue() {
		if ( $this->m_hDefValue != NULL )
			return new DoxyLink( $this->m_hDefValue );
		return NULL;
	}

	// Members
	private $m_hParam;
	private $m_hType;
	private $m_hDeclName;
	private $m_hDefName;
	private $m_hArray;
	private $m_hDefValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyEnumValue class
class DoxyEnumValue
{
	// Constructor / Destructor
    public function __construct( $hEnumValue )
    {
		assert( '$hEnumValue->GetTagName() == "enumvalue"' );
		$this->m_hEnumValue = $hEnumValue;

		$this->m_hName = $this->m_hEnumValue->GetChildByTag( "name" );
		assert( '$this->m_hName != NULL' );
		$this->m_hInitializer = $this->m_hEnumValue->GetChildByTag( "initializer" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetID() {
		assert( '$this->m_hEnumValue->HasAttribute("id")' );
		return $this->m_hEnumValue->GetAttributeValue( "id" );
	}

	public function GetProtection() {
		if ( $this->m_hEnumValue->HasAttribute("prot") )
			return $this->m_hEnumValue->GetAttributeValue( "prot" );
		return CODEDOC_XML_PROTECTION_PUBLIC;
	}

	// Children
	public function GetName() {
		return $this->m_hName->GetTextContent();
	}
	public function GetInitializer() {
		if ( $this->m_hInitializer != NULL )
			return new DoxyLink( $this->m_hInitializer );
		return "";
	}

	// Members
	private $m_hEnumValue;
	private $m_hName;
	private $m_hInitializer;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyLink class
class DoxyLink
{
	// Constructor / Destructor
    public function __construct( $hLink )
    {
		$this->m_hLink = $hLink;
		
		$this->m_arrReferences = $this->m_hLink->GetChildrenByTag( "ref" );
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Children
	public function GetText() {
		return $this->m_hLink->GetTextContent();
	}

	public function GetReferenceCount() {
		return count( $this->m_arrReferences );
	}
	public function GetReference( $i ) {
		return new DoxyReference( $this->m_arrReferences[$i] );
	}

	// Members
	private $m_hLink;
	private $m_arrReferences;
}

/////////////////////////////////////////////////////////////////////////////////
// The DoxyReference class
class DoxyReference
{
	// Constructor / Destructor
    public function __construct( $hReference )
    {
		assert( '$hReference->GetTagName() == "ref"' );
		$this->m_hReference = $hReference;
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Attributes
	public function GetTargetID() {
		assert( '$this->m_hReference->HasAttribute("refid")' );
		return $this->m_hReference->GetAttributeValue( "refid" );
	}
	public function GetTargetKind() {
		assert( '$this->m_hReference->HasAttribute("kindref")' );
		return $this->m_hReference->GetAttributeValue( "kindref" );
	}

	// Children
	public function GetText() {
		return $this->m_hReference->GetTextContent();
	}

	// Members
	private $m_hReference;
}
	
/////////////////////////////////////////////////////////////////////////////////
// The CodeDocGenerator class
class CodeDocGenerator
{
	// Constructor / Destructor
    public function __construct()
    {
		$this->m_arrFileStructure = array();
		$this->m_arrNamespaceStructure = array();
		$this->m_arrTypeStructure = array();
		$this->m_arrUnresolvedXReferences = array();
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Main routine
	public function GenerateStructure()
	{
		$hSystem = SystemManager::GetSingleton();
		$hCodeDoc = CodeDocManager::GetSingleton();

		$iInitialTimeLimit = $hSystem->GetInitialTimeLimit();
		$hSystem->SetTimeLimit( 0 );
		print_r( "<br/><br/>Generating documentation structure ..." );

		// Pass 1 : XML structure analysis
		$this->_AnalyseXML();
		print_r( "<br/>Pass 1 : Analysis Done !" );

		// Pass 2 : Generate Directories and Files
		foreach( $this->m_arrFileStructure as $strRefID => $arrData ) {
			if ( isset($arrData["parentdir"]) )
				continue;
			$this->_Rec_GenerateStructure_DirsFiles( $strRefID, CODEDOC_ID_NULL );
		}
		print_r( "<br/>Pass 2 : Generating Directories and Files Done !" );

		// Pass 3 : Generate Namespaces
		foreach( $this->m_arrNamespaceStructure as $strRefID => $arrData ) {
			if ( isset($arrData["parentnamespace"]) )
				continue;
			assert( 'isset( $arrData["parentfile"] )' );
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_Rec_GenerateStructure_Namespaces( $strRefID, $iParentFileID, CODEDOC_ID_NULL );
		}
		print_r( "<br/>Pass 3 : Generating Namespaces Done !" );

		// Pass 4 : Generate Types
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			if ( isset($arrData["parenttype"]) )
				continue;
			assert( 'isset( $arrData["parentfile"] )' );
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$iParentNamespaceID = CODEDOC_ID_NULL;
			if ( isset($arrData["parentnamespace"]) )
				$iParentNamespaceID = $this->m_arrNamespaceStructure[ $arrData["parentnamespace"] ]["NamespaceID"];
			$this->_Rec_GenerateStructure_Types( $strRefID, $iParentFileID, $iParentNamespaceID, CODEDOC_ID_NULL );
		}
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			if ( isset($arrData["basetypes"]) )
				$this->_ResolveBaseTypes( $arrData["TypeID"], $arrData["basetypes"] );
		}
		print_r( "<br/>Pass 4 : Generating Types Done !" );

		// Pass 5 : Generate Define members
		foreach( $this->m_arrFileStructure as $strRefID => $arrData ) {
			if ( $arrData["isdir"] )
				continue;
			$this->_GenerateMembers_Defines( $arrData["FileID"], $this->m_arrFileStructure[$strRefID]["members"] );
		}
		print_r( "<br/>Pass 5 : Generating Defines & Macros Done !" );

		// Pass 6 : Generate Enum members
		foreach( $this->m_arrFileStructure as $strRefID => $arrData ) {
			if ( $arrData["isdir"] )
				continue;
			$this->_GenerateMembers_Enums( $arrData["FileID"], CODEDOC_ID_NULL, CODEDOC_ID_NULL,
										   $this->m_arrFileStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrNamespaceStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Enums( $iParentFileID, $arrData["NamespaceID"], CODEDOC_ID_NULL,
										   $this->m_arrNamespaceStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Enums( $iParentFileID, CODEDOC_ID_NULL, $arrData["TypeID"],
										   $this->m_arrTypeStructure[$strRefID]["members"] );
		}
		print_r( "<br/>Pass 6 : Generating Enums Done !" );

		// Pass 7 : Generate Typedef members
		foreach( $this->m_arrFileStructure as $strRefID => $arrData ) {
			if ( $arrData["isdir"] )
				continue;
			$this->_GenerateMembers_Typedefs( $arrData["FileID"], CODEDOC_ID_NULL, CODEDOC_ID_NULL,
											  $this->m_arrFileStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrNamespaceStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Typedefs( $iParentFileID, $arrData["NamespaceID"], CODEDOC_ID_NULL,
											  $this->m_arrNamespaceStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Typedefs( $iParentFileID, CODEDOC_ID_NULL, $arrData["TypeID"],
											  $this->m_arrTypeStructure[$strRefID]["members"] );
		}
		print_r( "<br/>Pass 7 : Generating Typedefs Done !" );

		// Pass 8 : Generate Variable members
		foreach( $this->m_arrFileStructure as $strRefID => $arrData ) {
			if ( $arrData["isdir"] )
				continue;
			$this->_GenerateMembers_Variables( $arrData["FileID"], CODEDOC_ID_NULL, CODEDOC_ID_NULL,
											   $this->m_arrFileStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrNamespaceStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Variables( $iParentFileID, $arrData["NamespaceID"], CODEDOC_ID_NULL,
											   $this->m_arrNamespaceStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Variables( $iParentFileID, CODEDOC_ID_NULL, $arrData["TypeID"],
											   $this->m_arrTypeStructure[$strRefID]["members"] );
		}
		print_r( "<br/>Pass 8 : Generating Variables Done !" );

		// Pass 9 : Generate Function members
		foreach( $this->m_arrFileStructure as $strRefID => $arrData ) {
			if ( $arrData["isdir"] )
				continue;
			$this->_GenerateMembers_Functions( $arrData["FileID"], CODEDOC_ID_NULL, CODEDOC_ID_NULL,
											   $this->m_arrFileStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrNamespaceStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Functions( $iParentFileID, $arrData["NamespaceID"], CODEDOC_ID_NULL,
											   $this->m_arrNamespaceStructure[$strRefID]["members"] );
		}
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
			$this->_GenerateMembers_Functions( $iParentFileID, CODEDOC_ID_NULL, $arrData["TypeID"],
											   $this->m_arrTypeStructure[$strRefID]["members"] );
		}
		print_r( "<br/>Pass 9 : Generating Functions Done !" );

		// Pass 10 : Resolve all crossed references
		foreach( $this->m_arrUnresolvedXReferences as $iXReferenceID => $arrKindID ) {
			$arrXRefInfos = $this->_ResolveXReference( $arrKindID[0], $arrKindID[1] );
			assert( '$arrXRefInfos != NULL' );
			$strResolvedKind = $arrXRefInfos["targetkind"];
			$iResolvedID = $arrXRefInfos["targetid"];
			assert( '$iResolvedID != CODEDOC_XREFID_UNRESOLVED' );
			$hCodeDoc->ResolveXReference( $iXReferenceID, $strResolvedKind, $iResolvedID );
		}
		print_r( "<br/>Pass 10 : Resolving Crossed-References Done !" );
		
		// Done !!!
		$hSystem->SetTimeLimit( $iInitialTimeLimit );
		print_r( "<br/>Documentation structure generated successfully !<br/>" );
	}
	public function ImportContent()
	{
		$hSystem = SystemManager::GetSingleton();

		$iInitialTimeLimit = $hSystem->GetInitialTimeLimit();
		$hSystem->SetTimeLimit( 0 );

		$this->_DocData_ImportDocumentation();
		$this->_DocData_ImportReference();
		print_r( "<br/><br/>Importing documentation from XML Done !" );
		
		$hSystem->SetTimeLimit( $iInitialTimeLimit );
	}
	/*public function ExportContent()
	{
		$hSystem = SystemManager::GetSingleton();

		$iInitialTimeLimit = $hSystem->GetInitialTimeLimit();
		$hSystem->SetTimeLimit( 0 );
		
		$this->_DocData_Export();
		print_r( "<br/><br/>Exporting documentation to XML Done !" );
		
		$hSystem->SetTimeLimit( $iInitialTimeLimit );
	}*/

	// Helpers : Analysis
	private function _AnalyseXML()
	{
		// Get Index
		$hIndexXML = new DOxygenIndex();

		// Check version
		$strVersion = $hIndexXML->GetVersion();
		assert( '$strVersion == CODEDOC_XML_VERSION' );

		// Explore all compounds
		$iCompoundDeclCount = $hIndexXML->GetCompoundDeclCount();
		for( $i = 0; $i < $iCompoundDeclCount; ++$i ) {
			$hCompoundDecl = $hIndexXML->GetCompoundDecl( $i );

			$strCompoundDeclRefID = $hCompoundDecl->GetRefID();
			$strCompoundDeclKind = $hCompoundDecl->GetKind();
			$strCompoundDeclName = $hCompoundDecl->GetName();
			
			// Compound's XML file
			$hCompoundXML = new DOxygen( $strCompoundDeclRefID );

			// Check version
			$strVersion = $hCompoundXML->GetVersion();
			assert( '$strVersion == CODEDOC_XML_VERSION' );

			// Get CompoundDef
			$iCompoundDefCount = $hCompoundXML->GetCompoundDefCount();
			assert( '$iCompoundDefCount == 1' );
			$hCompoundDef = $hCompoundXML->GetCompoundDef( 0 );

			$strCompoundDefID = $hCompoundDef->GetID();
			$strCompoundDefKind = $hCompoundDef->GetKind();
			$strCompoundDefName = $hCompoundDef->GetName();
			$strLocationFile = $hCompoundDef->GetLocationFile();
			$iLocationLine = $hCompoundDef->GetLocationLine();

			$iPos = strpos( $strLocationFile, "/Scarab Engine/Source" );
			assert( '$iPos > 0' );
			$strLocationFile = substr( $strLocationFile, $iPos + 21 );

			// Check declaration
			assert( '$strCompoundDefID == $strCompoundDeclRefID' );
			assert( '$strCompoundDefKind == $strCompoundDeclKind' );
			assert( '$strCompoundDefName == $strCompoundDeclName' );

			// Kind-specific analysis
			switch( $strCompoundDefKind ) {
				case CODEDOC_XML_COMPOUNDKIND_DIR:
					// Gather data
					$this->m_arrFileStructure[$strCompoundDefID]["isdir"] = true;
					$this->m_arrFileStructure[$strCompoundDefID]["path"] = rtrim( $strLocationFile, "/" );
					$arrTmp = explode( "/", $strCompoundDefName );
					$iIndex = count($arrTmp) - 1;
					if ( $arrTmp[$iIndex] == "" )
						--$iIndex;
					$this->m_arrFileStructure[$strCompoundDefID]["name"] = $arrTmp[$iIndex];

					// Resolve parent links
					$iInnerDirCount = $hCompoundDef->GetInnerDirCount();
					for( $j = 0; $j < $iInnerDirCount; ++$j ) {
						$arrSubDir = $hCompoundDef->GetInnerDir( $j );
						$strSubDirRefID = $arrSubDir["refid"];
						$this->m_arrFileStructure[$strSubDirRefID]["parentdir"] = $strCompoundDefID;
					}
					$iInnerFileCount = $hCompoundDef->GetInnerFileCount();
					for( $j = 0; $j < $iInnerFileCount; ++$j ) {
						$arrSubFile = $hCompoundDef->GetInnerFile( $j );
						$strSubFileRefID = $arrSubFile["refid"];
						$this->m_arrFileStructure[$strSubFileRefID]["parentdir"] = $strCompoundDefID;
					}
					break;
				case CODEDOC_XML_COMPOUNDKIND_FILE:
					// Gather data
					$this->m_arrFileStructure[$strCompoundDefID]["isdir"] = false;
					$this->m_arrFileStructure[$strCompoundDefID]["path"] = $strLocationFile;
					$this->m_arrFileStructure[$strCompoundDefID]["name"] = $strCompoundDefName;

					// Resolve parent links
					$iInnerNamespaceCount = $hCompoundDef->GetInnerNamespaceCount();
					for( $j = 0; $j < $iInnerNamespaceCount; ++$j ) {
						$arrSubNamespace = $hCompoundDef->GetInnerNamespace( $j );
						$strSubNamespaceRefID = $arrSubNamespace["refid"];
						$this->m_arrNamespaceStructure[$strSubNamespaceRefID]["parentfile"] = $strCompoundDefID;
					}
					$iInnerTypeCount = $hCompoundDef->GetInnerTypeCount();
					for( $j = 0; $j < $iInnerTypeCount; ++$j ) {
						$arrSubType = $hCompoundDef->GetInnerType( $j );
						$strSubTypeRefID = $arrSubType["refid"];
						$this->m_arrTypeStructure[$strSubTypeRefID]["parentfile"] = $strCompoundDefID;
					}

					// Explore all members
					$this->m_arrFileStructure[$strCompoundDefID]["members"] = array();
					$this->_AnalyseExtract_Members( $this->m_arrFileStructure[$strCompoundDefID]["members"], $hCompoundDef );
					break;
				case CODEDOC_XML_COMPOUNDKIND_NAMESPACE:
					// Gather data
					$this->m_arrNamespaceStructure[$strCompoundDefID]["prot"] = $hCompoundDef->GetProtection();
					$this->m_arrNamespaceStructure[$strCompoundDefID]["name"] = $strCompoundDefName;
					$this->m_arrNamespaceStructure[$strCompoundDefID]["file"] = $strLocationFile;
					$this->m_arrNamespaceStructure[$strCompoundDefID]["line"] = $iLocationLine;

					// Resolve parent links
					$iInnerNamespaceCount = $hCompoundDef->GetInnerNamespaceCount();
					for( $j = 0; $j < $iInnerNamespaceCount; ++$j ) {
						$arrSubNamespace = $hCompoundDef->GetInnerNamespace( $j );
						$strSubNamespaceRefID = $arrSubNamespace["refid"];
						$this->m_arrNamespaceStructure[$strSubNamespaceRefID]["parentnamespace"] = $strCompoundDefID;
					}
					$iInnerTypeCount = $hCompoundDef->GetInnerTypeCount();
					for( $j = 0; $j < $iInnerTypeCount; ++$j ) {
						$arrSubType = $hCompoundDef->GetInnerType( $j );
						$strSubTypeRefID = $arrSubType["refid"];
						$this->m_arrTypeStructure[$strSubTypeRefID]["parentnamespace"] = $strCompoundDefID;
					}

					// Explore all members
					$this->m_arrNamespaceStructure[$strCompoundDefID]["members"] = array();
					$this->_AnalyseExtract_Members( $this->m_arrNamespaceStructure[$strCompoundDefID]["members"], $hCompoundDef );
					break;
				case CODEDOC_XML_COMPOUNDKIND_UNION:
				case CODEDOC_XML_COMPOUNDKIND_STRUCT:
				case CODEDOC_XML_COMPOUNDKIND_CLASS:
				case CODEDOC_XML_COMPOUNDKIND_INTERFACE:
					// Gather data
					$this->m_arrTypeStructure[$strCompoundDefID]["kind"] = $strCompoundDefKind;
					$this->m_arrTypeStructure[$strCompoundDefID]["prot"] = $hCompoundDef->GetProtection();
					$this->m_arrTypeStructure[$strCompoundDefID]["name"] = $strCompoundDefName;
					$this->m_arrTypeStructure[$strCompoundDefID]["file"] = $strLocationFile;
					$this->m_arrTypeStructure[$strCompoundDefID]["line"] = $iLocationLine;

					$this->m_arrTypeStructure[$strCompoundDefID]["basetypes"] = array();
					$iBaseTypeCount = $hCompoundDef->GetBaseTypeCount();
					for( $j = 0; $j < $iBaseTypeCount; ++$j ) {
						$arrBaseType = $hCompoundDef->GetBaseType( $j );
						array_push( $this->m_arrTypeStructure[$strCompoundDefID]["basetypes"], $arrBaseType );
					}

					$this->m_arrTypeStructure[$strCompoundDefID]["templateparams"] = array();
					$this->_AnalyseExtract_TemplateParams( $this->m_arrTypeStructure[$strCompoundDefID]["templateparams"], $hCompoundDef );

					// Resolve parent links
					$iInnerTypeCount = $hCompoundDef->GetInnerTypeCount();
					for( $j = 0; $j < $iInnerTypeCount; ++$j ) {
						$arrSubType = $hCompoundDef->GetInnerType( $j );
						$strSubTypeRefID = $arrSubType["refid"];
						$this->m_arrTypeStructure[$strSubTypeRefID]["parenttype"] = $strCompoundDefID;
					}

					// Explore all members
					$this->m_arrTypeStructure[$strCompoundDefID]["members"] = array();
					$this->_AnalyseExtract_Members( $this->m_arrTypeStructure[$strCompoundDefID]["members"], $hCompoundDef );
					break;
				default: break;
			}
		}

		// Solve nested types from template specialization
		foreach( $this->m_arrTypeStructure as $strRefID => $arrData ) {
			if ( isset($arrData["parentfile"]) )
				continue;

			// Get the template version of this specialization, no other choice
			// here than a dirty name hack ...
			// A cleaner & better choice would require not using doxygen's directories,
			// replacing "class" with "classT" would be enough to get the template's id
			// without a search loop ... still a name hack though ...
			$strSpecializedName = $arrData["name"];
			$strTemplateName = "T" . $strSpecializedName;

			$strTemplateRefID = NULL;
			foreach( $this->m_arrTypeStructure as $strTRefID => $arrTData ) {
				if ( $arrTData["name"] == $strTemplateName ) {
					$strTemplateRefID = $strTRefID;
					break;
				}
			}
			assert( '$strTemplateRefID != NULL' );

			$this->m_arrTypeStructure[$strRefID]["parentfile"] = $this->m_arrTypeStructure[$strTemplateRefID]["parentfile"];
			if ( isset($this->m_arrTypeStructure[$strTemplateRefID]["parentnamespace"]) )
				$this->m_arrTypeStructure[$strRefID]["parentnamespace"] = $this->m_arrTypeStructure[$strTemplateRefID]["parentnamespace"];
			if ( isset($this->m_arrTypeStructure[$strTemplateRefID]["parenttype"]) )
				$this->m_arrTypeStructure[$strRefID]["parenttype"] = $this->m_arrTypeStructure[$strTemplateRefID]["parenttype"];
		}
	}
	private function _AnalyseExtract_Members( & $arrMembers, $hCompoundDef )
	{
		// Explore all sections
		$iSectionDefCount = $hCompoundDef->GetSectionDefCount();
		for( $i = 0; $i < $iSectionDefCount; ++$i ) {
			$hSectionDef = $hCompoundDef->GetSectionDef( $i );

			// Explore all members
			$iMemberDefCount = $hSectionDef->GetMemberDefCount();
			for( $j = 0; $j < $iMemberDefCount; ++$j ) {
				$hMemberDef = $hSectionDef->GetMemberDef( $j );

				$strMemberDefID = $hMemberDef->GetID();
				$strMemberDefKind = $hMemberDef->GetKind();
				$strMemberDefName = $hMemberDef->GetName();
				$strLocationFile = $hCompoundDef->GetLocationFile();
				$iLocationLine = $hCompoundDef->GetLocationLine();
				
				$iPos = strpos( $strLocationFile, "/Scarab Engine/Source" );
				assert( '$iPos > 0' );
				$strLocationFile = substr( $strLocationFile, $iPos + 21 );

				// Kind-specific analysis
				switch( $strMemberDefKind ) {
					case CODEDOC_XML_MEMBERKIND_DEFINE:
						$arrMembers[$strMemberDefID]["kind"] = $strMemberDefKind;
						$arrMembers[$strMemberDefID]["prot"] = $hMemberDef->GetProtection();
						$arrMembers[$strMemberDefID]["name"] = $strMemberDefName;
						$arrMembers[$strMemberDefID]["file"] = $strLocationFile;
						$arrMembers[$strMemberDefID]["line"] = $iLocationLine;

						$hInitializer = $hMemberDef->GetInitializer();
						$arrMembers[$strMemberDefID]["initializer_text"] = "";
						$arrMembers[$strMemberDefID]["initializer_refs"] = array();
						if ( $hInitializer != NULL ) {
							$arrMembers[$strMemberDefID]["initializer_text"] = $hInitializer->GetText();
							$this->_AnalyseExtract_References( $arrMembers[$strMemberDefID]["initializer_refs"], $hInitializer );
						}

						$arrMembers[$strMemberDefID]["params"] = array();
						$this->_AnalyseExtract_Params( $arrMembers[$strMemberDefID]["params"], $hMemberDef );
						break;
					case CODEDOC_XML_MEMBERKIND_ENUM:
						$arrMembers[$strMemberDefID]["kind"] = $strMemberDefKind;
						$arrMembers[$strMemberDefID]["prot"] = $hMemberDef->GetProtection();
						$arrMembers[$strMemberDefID]["name"] = $strMemberDefName;
						$arrMembers[$strMemberDefID]["file"] = $strLocationFile;
						$arrMembers[$strMemberDefID]["line"] = $iLocationLine;

						$arrMembers[$strMemberDefID]["enumvalues"] = array();
						$this->_AnalyseExtract_EnumValues( $arrMembers[$strMemberDefID]["enumvalues"], $hMemberDef );
						break;
					case CODEDOC_XML_MEMBERKIND_TYPEDEF:
						$arrMembers[$strMemberDefID]["kind"] = $strMemberDefKind;
						$arrMembers[$strMemberDefID]["prot"] = $hMemberDef->GetProtection();
						$arrMembers[$strMemberDefID]["name"] = $strMemberDefName;
						$arrMembers[$strMemberDefID]["file"] = $strLocationFile;
						$arrMembers[$strMemberDefID]["line"] = $iLocationLine;

						$arrMembers[$strMemberDefID]["definition"] = $hMemberDef->GetDefinition();
						$arrMembers[$strMemberDefID]["argstring"] = $hMemberDef->GetArgString();

						$hType = $hMemberDef->GetType();
						$arrMembers[$strMemberDefID]["type_text"] = "";
						$arrMembers[$strMemberDefID]["type_refs"] = array();
						if ( $hType != NULL ) {
							$arrMembers[$strMemberDefID]["type_text"] = $hType->GetText();
							$this->_AnalyseExtract_References( $arrMembers[$strMemberDefID]["type_refs"], $hType );
						}

						$arrMembers[$strMemberDefID]["params"] = array();
						$this->_AnalyseExtract_Params( $arrMembers[$strMemberDefID]["params"], $hMemberDef );
						break;
					case CODEDOC_XML_MEMBERKIND_VARIABLE:
						$arrMembers[$strMemberDefID]["kind"] = $strMemberDefKind;
						$arrMembers[$strMemberDefID]["prot"] = $hMemberDef->GetProtection();
						$arrMembers[$strMemberDefID]["name"] = $strMemberDefName;
						$arrMembers[$strMemberDefID]["file"] = $strLocationFile;
						$arrMembers[$strMemberDefID]["line"] = $iLocationLine;

						$arrMembers[$strMemberDefID]["static"] = $hMemberDef->IsStatic();
						$arrMembers[$strMemberDefID]["const"] = $hMemberDef->IsConst();
						$arrMembers[$strMemberDefID]["volatile"] = $hMemberDef->IsVolatile();
						$arrMembers[$strMemberDefID]["mutable"] = $hMemberDef->IsMutable();

						$arrMembers[$strMemberDefID]["definition"] = $hMemberDef->GetDefinition();

						$arrMembers[$strMemberDefID]["templateparams"] = array();
						$this->_AnalyseExtract_TemplateParams( $arrMembers[$strMemberDefID]["templateparams"], $hMemberDef );

						$hType = $hMemberDef->GetType();
						$arrMembers[$strMemberDefID]["type_text"] = "";
						$arrMembers[$strMemberDefID]["type_refs"] = array();
						if ( $hType != NULL ) {
							$arrMembers[$strMemberDefID]["type_text"] = $hType->GetText();
							$this->_AnalyseExtract_References( $arrMembers[$strMemberDefID]["type_refs"], $hType );
						}

						$hInitializer = $hMemberDef->GetInitializer();
						$arrMembers[$strMemberDefID]["initializer_text"] = "";
						$arrMembers[$strMemberDefID]["initializer_refs"] = array();
						if ( $hInitializer != NULL ) {
							$arrMembers[$strMemberDefID]["initializer_text"] = $hInitializer->GetText();
							$this->_AnalyseExtract_References( $arrMembers[$strMemberDefID]["initializer_refs"], $hInitializer );
						}
						break;
					case CODEDOC_XML_MEMBERKIND_FUNCTION:
						$arrMembers[$strMemberDefID]["kind"] = $strMemberDefKind;
						$arrMembers[$strMemberDefID]["prot"] = $hMemberDef->GetProtection();
						$arrMembers[$strMemberDefID]["name"] = $strMemberDefName;
						$arrMembers[$strMemberDefID]["file"] = $strLocationFile;
						$arrMembers[$strMemberDefID]["line"] = $iLocationLine;

						$arrMembers[$strMemberDefID]["inline"] = $hMemberDef->IsInline();
						$arrMembers[$strMemberDefID]["static"] = $hMemberDef->IsStatic();
						$arrMembers[$strMemberDefID]["virtual"] = $hMemberDef->GetVirtual();
						$arrMembers[$strMemberDefID]["const"] = $hMemberDef->IsConst();

						$arrMembers[$strMemberDefID]["definition"] = $hMemberDef->GetDefinition();
						$arrMembers[$strMemberDefID]["argstring"] = $hMemberDef->GetArgString();

						$arrMembers[$strMemberDefID]["templateparams"] = array();
						$this->_AnalyseExtract_TemplateParams( $arrMembers[$strMemberDefID]["templateparams"], $hMemberDef );

						$hType = $hMemberDef->GetType();
						$arrMembers[$strMemberDefID]["type_text"] = "";
						$arrMembers[$strMemberDefID]["type_refs"] = array();
						if ( $hType != NULL ) {
							$arrMembers[$strMemberDefID]["type_text"] = $hType->GetText();
							$this->_AnalyseExtract_References( $arrMembers[$strMemberDefID]["type_refs"], $hType );
						}

						$arrMembers[$strMemberDefID]["params"] = array();
						$this->_AnalyseExtract_Params( $arrMembers[$strMemberDefID]["params"], $hMemberDef );
						break;
					default:
						// Ignore ...
						break;
				}
			}
		}
	}
	private function _AnalyseExtract_EnumValues( & $arrEnumValues, $hMemberDef )
	{
		$iEnumValueCount = $hMemberDef->GetEnumValueCount();
		for( $i = 0; $i < $iEnumValueCount; ++$i ) {
			$hEnumValue = $hMemberDef->GetEnumValue( $i );
			$strName = $hEnumValue->GetName();	
			$hInitializer = $hEnumValue->GetInitializer();	

			$arrEnumValues[$strName] = array();
			$arrEnumValues[$strName]["initializer_text"] = "";
			$arrEnumValues[$strName]["initializer_refs"] = array();
			if ( $hInitializer != NULL ) {
				$arrEnumValues[$strName]["initializer_text"] = $hInitializer->GetText();
				$this->_AnalyseExtract_References( $arrEnumValues[$strName]["initializer_refs"], $hInitializer );
			}
		}
	}
	private function _AnalyseExtract_TemplateParams( & $arrTemplateParams, $hMemberDef )
	{
		$iTemplateParamCount = $hMemberDef->GetTemplateParamCount();
		for( $i = 0; $i < $iTemplateParamCount; ++$i ) {
			$hTemplateParam = $hMemberDef->GetTemplateParam( $i );

			$arrTemplateParams[$i] = array();
			$arrTemplateParams[$i]["name"] = $hTemplateParam->GetDeclarationName();
			if ( $arrTemplateParams[$i]["name"] == "" )
				$arrTemplateParams[$i]["name"] = $hTemplateParam->GetDefinitionName();

			$hType = $hTemplateParam->GetType();
			$arrTemplateParams[$i]["type_text"] = "";
			$arrTemplateParams[$i]["type_refs"] = array();
			if ( $hType != NULL ) {
				$arrTemplateParams[$i]["type_text"] = $hType->GetText();
				$this->_AnalyseExtract_References( $arrTemplateParams[$i]["type_refs"], $hType );
			}

			$hDefVal = $hTemplateParam->GetDefaultValue();
			$arrTemplateParams[$i]["defval_text"] = "";
			$arrTemplateParams[$i]["defval_refs"] = array();
			if ( $hDefVal != NULL ) {
				$arrTemplateParams[$i]["defval_text"] = $hDefVal->GetText();
				$this->_AnalyseExtract_References( $arrTemplateParams[$i]["defval_refs"], $hDefVal );
			}
		}
	}
	private function _AnalyseExtract_Params( & $arrParams, $hMemberDef )
	{
		$iParamCount = $hMemberDef->GetParamCount();
		for( $i = 0; $i < $iParamCount; ++$i ) {
			$hParam = $hMemberDef->GetParam( $i );
			
			$arrParams[$i] = array();
			$arrParams[$i]["name"] = $hParam->GetDeclarationName();
			if ( $arrParams[$i]["name"] == "" )
				$arrParams[$i]["name"] = $hParam->GetDefinitionName();

			$hType = $hParam->GetType();
			$arrParams[$i]["type_text"] = "";
			$arrParams[$i]["type_refs"] = array();
			if ( $hType != NULL ) {
				$arrParams[$i]["type_text"] = $hType->GetText();
				$this->_AnalyseExtract_References( $arrParams[$i]["type_refs"], $hType );
			}

			$hDefVal = $hParam->GetDefaultValue();
			$arrParams[$i]["defval_text"] = "";
			$arrParams[$i]["defval_refs"] = array();
			if ( $hDefVal != NULL ) {
				$arrParams[$i]["defval_text"] = $hDefVal->GetText();
				$this->_AnalyseExtract_References( $arrParams[$i]["defval_refs"], $hDefVal );
			}
		}
	}
	private function _AnalyseExtract_References( & $arrReferences, $hLink )
	{
		$iReferenceCount = $hLink->GetReferenceCount();
		for( $i = 0; $i < $iReferenceCount; ++$i ) {
			$hReference = $hLink->GetReference( $i );
			array_push( $arrReferences, array( "targetkind" => $hReference->GetTargetKind(),
											   "targetid" => $hReference->GetTargetID(),
											   "reftext" => $hReference->GetText() ) );
		}
	}

	// Helpers : Generator
	private function _Rec_GenerateStructure_DirsFiles( $strRefID, $iParentDirID )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		if ( $this->m_arrFileStructure[$strRefID]["isdir"] ) {
			$iGeneratedID = $hCodeDoc->CreateDirectory( $iParentDirID,
														$this->m_arrFileStructure[$strRefID]["name"],
														$this->m_arrFileStructure[$strRefID]["path"],
														CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
			$this->m_arrFileStructure[$strRefID]["DirectoryID"] = $iGeneratedID;

			foreach( $this->m_arrFileStructure as $strSubRefID => $arrData ) {
				if ( isset($arrData["parentdir"]) && $arrData["parentdir"] == $strRefID )
					$this->_Rec_GenerateStructure_DirsFiles( $strSubRefID, $iGeneratedID );
			}
		} else {
			$iGeneratedID = $hCodeDoc->CreateFile( $iParentDirID,
												   $this->m_arrFileStructure[$strRefID]["name"],
												   $this->m_arrFileStructure[$strRefID]["path"],
												   CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
			$this->m_arrFileStructure[$strRefID]["FileID"] = $iGeneratedID;
		}
	}
	private function _Rec_GenerateStructure_Namespaces( $strRefID, $iParentFileID, $iParentNamespaceID )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		$iGeneratedID = $hCodeDoc->CreateNamespace( $iParentFileID, $iParentNamespaceID,
													$this->m_arrNamespaceStructure[$strRefID]["name"],
													$this->m_arrNamespaceStructure[$strRefID]["file"],
													$this->m_arrNamespaceStructure[$strRefID]["line"],
													$this->m_arrNamespaceStructure[$strRefID]["prot"],
													CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
		$this->m_arrNamespaceStructure[$strRefID]["NamespaceID"] = $iGeneratedID;

		foreach( $this->m_arrNamespaceStructure as $strSubRefID => $arrData ) {
			if ( isset($arrData["parentnamespace"]) && $arrData["parentnamespace"] == $strRefID ) {
				$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
				$this->_Rec_GenerateStructure_Namespaces( $strSubRefID, $iParentFileID, $iGeneratedID );
			}
		}
	}
	private function _Rec_GenerateStructure_Types( $strRefID, $iParentFileID, $iParentNamespaceID, $iParentTypeID )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		$arrTemplateParams = array();
		$this->_GenerateParameters( $arrTemplateParams, $this->m_arrTypeStructure[$strRefID]["templateparams"] );

		$iGeneratedID = $hCodeDoc->CreateType( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
											   $this->m_arrTypeStructure[$strRefID]["name"],
											   $this->m_arrTypeStructure[$strRefID]["file"],
											   $this->m_arrTypeStructure[$strRefID]["line"],
											   array(), $arrTemplateParams,
											   $this->m_arrTypeStructure[$strRefID]["prot"],
											   $this->m_arrTypeStructure[$strRefID]["kind"],
											   CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
		$this->m_arrTypeStructure[$strRefID]["TypeID"] = $iGeneratedID;

		foreach( $this->m_arrTypeStructure as $strSubRefID => $arrData ) {
			if ( isset($arrData["parenttype"]) && $arrData["parenttype"] == $strRefID ) {
				$iParentFileID = $this->m_arrFileStructure[ $arrData["parentfile"] ]["FileID"];
				$iParentNamespaceID = CODEDOC_ID_NULL;
				if ( isset($arrData["parentnamespace"]) )
					$iParentNamespaceID = $this->m_arrNamespaceStructure[ $arrData["parentnamespace"] ]["NamespaceID"];
				$this->_Rec_GenerateStructure_Types( $strSubRefID, $iParentFileID, $iParentNamespaceID, $iGeneratedID );
			}
		}
	}

	private function _GenerateMembers_Defines( $iParentFileID, & $arrMembers )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Explore all define members
		foreach( $arrMembers as $strMemberID => $arrMemberData ) {
			if ( $arrMemberData["kind"] != CODEDOC_XML_MEMBERKIND_DEFINE )
				continue;

			$iParamCount = count( $arrMemberData["params"] );
			$arrMembers[$strMemberID]["ismacro"] = ( $iParamCount > 0 );

			if ( $arrMembers[$strMemberID]["ismacro"] ) {
				$arrParams = array();
				$this->_GenerateParameters( $arrParams, $arrMemberData["params"] );
				$iBodyXLinkID = $this->_GenerateXReferences( $arrMemberData["initializer_text"], $arrMemberData["initializer_refs"] );

				$iGeneratedID = $hCodeDoc->CreateMacro( $iParentFileID,
														$arrMemberData["name"],
														$arrMemberData["file"],
														$arrMemberData["line"],
														$arrParams, $iBodyXLinkID,
														CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
				$arrMembers[$strMemberID]["MacroID"] = $iGeneratedID;
			} else {
				$iBodyXLinkID = $this->_GenerateXReferences( $arrMemberData["initializer_text"], $arrMemberData["initializer_refs"] );

				$iGeneratedID = $hCodeDoc->CreateDefine( $iParentFileID,
														 $arrMemberData["name"],
														 $arrMemberData["file"],
														 $arrMemberData["line"],
														 $iBodyXLinkID,
														 CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
				$arrMembers[$strMemberID]["DefineID"] = $iGeneratedID;
			}
		}
	}
	private function _GenerateMembers_Enums( $iParentFileID, $iParentNamespaceID, $iParentTypeID, & $arrMembers )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Explore all enum members
		foreach( $arrMembers as $strMemberID => $arrMemberData ) {
			if ( $arrMemberData["kind"] != CODEDOC_XML_MEMBERKIND_ENUM )
				continue;

			$arrEnumValues = array();
			foreach( $arrMemberData["enumvalues"] as $strName => $arrInitializer ) {
				$arrEnumValues[$strName] = $this->_GenerateXReferences( $arrInitializer["initializer_text"],
																		$arrInitializer["initializer_refs"] );
			}

			$iGeneratedID = $hCodeDoc->CreateEnum( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
												   $arrMemberData["name"],
								   				   $arrMemberData["file"],
								   				   $arrMemberData["line"],
												   $arrMemberData["prot"],
												   $arrEnumValues,
												   CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
			$arrMembers[$strMemberID]["EnumID"] = $iGeneratedID;			
		}
	}
	private function _GenerateMembers_Typedefs( $iParentFileID, $iParentNamespaceID, $iParentTypeID, & $arrMembers )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Explore all typedef members
		foreach( $arrMembers as $strMemberID => $arrMemberData ) {
			if ( $arrMemberData["kind"] != CODEDOC_XML_MEMBERKIND_TYPEDEF )
				continue;

			$iTargetTypeXLinkID = $this->_GenerateXReferences( $arrMemberData["type_text"], $arrMemberData["type_refs"] );
			$arrParams = array();
			$this->_GenerateParameters( $arrParams, $arrMemberData["params"] );

			$iGeneratedID = $hCodeDoc->CreateTypedef( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
													  $arrMemberData["name"],
													  $arrMemberData["file"],
													  $arrMemberData["line"],
													  $arrMemberData["prot"],
													  $iTargetTypeXLinkID, $arrParams, 
													  CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
			$arrMembers[$strMemberID]["TypedefID"] = $iGeneratedID;
		}
	}
	private function _GenerateMembers_Variables( $iParentFileID, $iParentNamespaceID, $iParentTypeID, & $arrMembers )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Explore all variable members
		foreach( $arrMembers as $strMemberID => $arrMemberData ) {
			if ( $arrMemberData["kind"] != CODEDOC_XML_MEMBERKIND_VARIABLE )
				continue;

			$arrTemplateParams = array();
			$this->_GenerateParameters( $arrTemplateParams, $arrMemberData["templateparams"] );
			$iInstanceTypeXLinkID = $this->_GenerateXReferences( $arrMemberData["type_text"], $arrMemberData["type_refs"] );
			$iInitializerXLinkID = $this->_GenerateXReferences( $arrMemberData["initializer_text"], $arrMemberData["initializer_refs"] );

			$iGeneratedID = $hCodeDoc->CreateVariable( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
													   $arrMemberData["name"],
													   $arrMemberData["file"],
													   $arrMemberData["line"],
													   $arrTemplateParams, $arrMemberData["prot"],
													   $arrMemberData["static"], $arrMemberData["const"],
													   $arrMemberData["volatile"], $arrMemberData["mutable"],
													   $iInstanceTypeXLinkID, $iInitializerXLinkID,
													   CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
			$arrMembers[$strMemberID]["VariableID"] = $iGeneratedID;
		}
	}
	private function _GenerateMembers_Functions( $iParentFileID, $iParentNamespaceID, $iParentTypeID, & $arrMembers )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Explore all function members
		foreach( $arrMembers as $strMemberID => $arrMemberData ) {
			if ( $arrMemberData["kind"] != CODEDOC_XML_MEMBERKIND_FUNCTION )
				continue;

			$arrTemplateParams = array();
			$this->_GenerateParameters( $arrTemplateParams, $arrMemberData["templateparams"] );
			$iReturnTypeXLinkID = $this->_GenerateXReferences( $arrMemberData["type_text"], $arrMemberData["type_refs"] );
			$arrParams = array();
			$this->_GenerateParameters( $arrParams, $arrMemberData["params"] );

			$iGeneratedID = $hCodeDoc->CreateFunction( $iParentFileID, $iParentNamespaceID, $iParentTypeID,
													   $arrMemberData["name"],
													   $arrMemberData["file"],
													   $arrMemberData["line"],
													   $arrTemplateParams, $arrMemberData["prot"],
													   $arrMemberData["inline"], $arrMemberData["static"],
													   $arrMemberData["virtual"], $arrMemberData["const"],
													   $iReturnTypeXLinkID, $arrParams,
													   CODEDOC_SAMPLE_SHORT_DESC, CODEDOC_SAMPLE_DOC_CONTENT );
			$arrMembers[$strMemberID]["FunctionID"] = $iGeneratedID;
		}
	}

	private function _ResolveBaseTypes( $iTypeID, $arrBaseTypes )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		$arrTemp = array();

		$iBaseTypeCount = count( $arrBaseTypes );
		for( $i = 0; $i < $iBaseTypeCount; ++$i ) {
			$arrBaseTypeInfos = $arrBaseTypes[$i];
			$strBaseTypeRefID = $arrBaseTypeInfos["refid"];
			$strBaseTypeProt = $arrBaseTypeInfos["prot"];
			$strBaseTypeVirtual = $arrBaseTypeInfos["virtual"];

			$iBaseTypeID = $this->m_arrTypeStructure[$strBaseTypeRefID]["TypeID"];
			$strDerivMethod = ( $strBaseTypeVirtual == CODEDOC_XML_VIRTUAL_YES ) ? "virtual_".$strBaseTypeProt
																				  : $strBaseTypeProt;

			$arrTemp[$iBaseTypeID] = $strDerivMethod;
		}

		$hCodeDoc->ResolveBaseTypes( $iTypeID, $arrTemp );
	}

	private function _GenerateParameters( & $arrParams, $arrMemberParams )
	{
		$iParamCount = count( $arrMemberParams );
		for( $i = 0; $i < $iParamCount; ++$i ) {
			$arrParamData = $arrMemberParams[$i];
			$iTypeXLinkID = $this->_GenerateXReferences( $arrParamData["type_text"], $arrParamData["type_refs"] );
			$iDefValXLinkID = $this->_GenerateXReferences( $arrParamData["defval_text"], $arrParamData["defval_refs"] );
			array_push( $arrParams, array( $arrParamData["name"], $iTypeXLinkID, $iDefValXLinkID ) );
		}
	}
	private function _GenerateXReferences( $strLinkText, $arrReferences )
	{
		$hCodeDoc = CodeDocManager::GetSingleton();

		$iXLinkID = $hCodeDoc->CreateXLink( $strLinkText );

		$iRefCount = count( $arrReferences );
		for( $i = 0; $i < $iRefCount; ++$i ) {//$arrReferences as $strXRefID => $strXRefKind ) {
			$arrReferenceData = $arrReferences[$i];

			$arrXRefInfos = $this->_ResolveXReference( $arrReferenceData["targetkind"], $arrReferenceData["targetid"] );
			if ( $arrXRefInfos == NULL )
				continue;

			$iXRefID = $hCodeDoc->CreateXReference( $iXLinkID, $arrXRefInfos["targetkind"], $arrXRefInfos["targetid"],
															   $arrReferenceData["reftext"] );

			if ( $arrXRefInfos["targetid"] == CODEDOC_XREFID_UNRESOLVED )
				$this->m_arrUnresolvedXReferences[$iXRefID] = array( $arrReferenceData["targetkind"], $arrReferenceData["targetid"] );
		}

		return $iXLinkID;
	}
	private function _ResolveXReference( $strXRefKind, $strXRefID )
	{
		if ( $strXRefKind == "compound" ) {
			$iTargetID = CODEDOC_XREFID_UNRESOLVED;
			if ( isset($this->m_arrTypeStructure[$strXRefID]["TypeID"]) )
				$iTargetID = $this->m_arrTypeStructure[$strXRefID]["TypeID"];

			return array( "targetkind" => CODEDOC_XREFKIND_TYPE,
						  "targetid" => $iTargetID );
		}

		assert( $strXRefKind == "member" );

		$arrSplitted = explode( "_1a", $strXRefID );
		$strCompoundRefID = $arrSplitted[0];
		$strMemberRefID = $strXRefID;

		$arrMemberData = NULL;
		if ( isset($this->m_arrFileStructure[$strCompoundRefID]) ) {
			if ( !isset($this->m_arrFileStructure[$strCompoundRefID]["members"][$strMemberRefID]) )
				return NULL;
			$arrMemberData = & $this->m_arrFileStructure[$strCompoundRefID]["members"][$strMemberRefID];
		} else if ( isset($this->m_arrNamespaceStructure[$strCompoundRefID]) ) {
			if ( !isset($this->m_arrNamespaceStructure[$strCompoundRefID]["members"][$strMemberRefID]) )
				return NULL;
			$arrMemberData = & $this->m_arrNamespaceStructure[$strCompoundRefID]["members"][$strMemberRefID];
		} else {
			assert( 'isset($this->m_arrTypeStructure[$strCompoundRefID])' );
			if ( !isset($this->m_arrTypeStructure[$strCompoundRefID]["members"][$strMemberRefID]) )
				return NULL;
			$arrMemberData = & $this->m_arrTypeStructure[$strCompoundRefID]["members"][$strMemberRefID];
		}

		$strTargetKind = "";
		$iTargetID = CODEDOC_XREFID_UNRESOLVED;
		switch( $arrMemberData["kind"] ) {
			case CODEDOC_XML_MEMBERKIND_DEFINE:
				if ( isset($arrMemberData["DefineID"]) ) {
					$strTargetKind = CODEDOC_XREFKIND_DEFINE;
					$iTargetID = $arrMemberData["DefineID"];
				} else if ( isset($arrMemberData["MacroID"]) ) {
					$strTargetKind = CODEDOC_XREFKIND_MACRO;
					$iTargetID = $arrMemberData["MacroID"];
				}
				break;
			case CODEDOC_XML_MEMBERKIND_ENUM:
				if ( isset($arrMemberData["EnumID"]) ) {
					$strTargetKind = CODEDOC_XREFKIND_ENUM;
					$iTargetID = $arrMemberData["EnumID"];
				}
				break;
			case CODEDOC_XML_MEMBERKIND_TYPEDEF:
				if ( isset($arrMemberData["TypedefID"]) ) {
					$strTargetKind = CODEDOC_XREFKIND_TYPEDEF;
					$iTargetID = $arrMemberData["TypedefID"];
				}
				break;
			case CODEDOC_XML_MEMBERKIND_VARIABLE:
				if ( isset($arrMemberData["VariableID"]) ) {
					$strTargetKind = CODEDOC_XREFKIND_VARIABLE;
					$iTargetID = $arrMemberData["VariableID"];
				}
				break;
			case CODEDOC_XML_MEMBERKIND_FUNCTION:
				if ( isset($arrMemberData["FunctionID"]) ) {
					$strTargetKind = CODEDOC_XREFKIND_FUNCTION;
					$iTargetID = $arrMemberData["FunctionID"];
				}
				break;
			default: assert('false'); break;
		}

		// Return target's Kind & ID
		return array( "targetkind" => $strTargetKind,
					  "targetid" => $iTargetID );
	}
	
	// Helpers : Documentation data
	private function _DocData_Get( $hElement, $strLang = "en" )
	{
		$arrChildren = $hElement->GetChildrenByTag( "shortdesc" );
		assert( 'count($arrChildren) == 1' );
		$hShortDesc = $arrChildren[0];

		$arrChildren = $hElement->GetChildrenByTag( "doccontent" );
		assert( 'count($arrChildren) == 1' );
		$hDocContent = $arrChildren[0];
		
		$arrChildren = $hShortDesc->GetChildrenByTag( "lang-" . $strLang );
		assert( 'count($arrChildren) == 1' );
		$hShortDescLang = $arrChildren[0];

		$arrChildren = $hDocContent->GetChildrenByTag( "lang-" . $strLang );
		assert( 'count($arrChildren) == 1' );
		$hDocContentLang = $arrChildren[0];
		
		return array( $hShortDescLang, $hDocContentLang );
	}
	
	private function _DocData_ImportDocumentation()
	{
		$hCodeDoc = CodeDocManager::GetSingleton();
		$hXML = XMLManager::GetSingleton();

		$strContentPath = CODEDOC_XML_ROOTPATH . "/Content";
		
		$hDocument = $hXML->CreateDocument();
		$bOk = $hDocument->LoadFromFile( $strContentPath . "/Root.xml" );
		if ( !$bOk )
			return;
		$hRoot = $hDocument->GetRoot();

		$arrChildren = $hRoot->GetChildrenByTag( "documentation" );
		assert( 'count($arrChildren) == 1' );
		$hDocumentation = $arrChildren[0];
		
		$arrDocData = $this->_DocData_Get( $hDocumentation, "en" );
		$hCodeDoc->UpdateDocumentation( "Root", $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		
		$strContentPath .= "/Root";

		$arrTemp = array( "GettingStarted", "CodingConventions", "BasicFramework", "DevelopperManual", "Reference" );
		foreach( $arrTemp as $strDoc ) {
			$hDocument = $hXML->CreateDocument();
			$bOk = $hDocument->LoadFromFile( $strContentPath . "/" . $strDoc . ".xml" );
			if ( !$bOk )
				return;
			$hRoot = $hDocument->GetRoot();

			$arrChildren = $hRoot->GetChildrenByTag( "documentation" );
			assert( 'count($arrChildren) == 1' );
			$hDocumentation = $arrChildren[0];
			
			$arrDocData = $this->_DocData_Get( $hDocumentation, "en" );
			$hCodeDoc->UpdateDocumentation( $strDoc, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
	}
	private function _DocData_ImportReference()
	{	
		$hCodeDoc = CodeDocManager::GetSingleton();
		
		$strContentPath = CODEDOC_XML_ROOTPATH . "/Content/Root/Reference";
		
		$arrDirectories = $hCodeDoc->ListDirectories( CODEDOC_ID_NULL, true );
		$iDirectoryCount = count( $arrDirectories );
		for( $i = 0; $i < $iDirectoryCount; ++$i ) {
			$arrDirectoryRow = $arrDirectories[$i];
			$iDirectoryID = $arrDirectoryRow["DirectoryID"];
			$strName = $arrDirectoryRow["Name"];
			$this->_DocData_Import_Directory( $iDirectoryID, $strName, $strContentPath );
		}
	}
	private function _DocData_Import_Directory( $iDirectoryID, $strName, $strContentPath )
	{
		$hXML = XMLManager::GetSingleton();
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Open XML file
		$hDocument = $hXML->CreateDocument();
		$bOk = $hDocument->LoadFromFile( $strContentPath . "/$strName" . ".xml" );
		if ( !$bOk )
			return;
		$hRoot = $hDocument->GetRoot();
		
		// Fill directory data
		$arrChildren = $hRoot->GetChildrenByTag( "directory" );
		assert( 'count($arrChildren) == 1' );
		$hDirectory = $arrChildren[0];
		
		$arrDocData = $this->_DocData_Get( $hDirectory, "en" );
		$hCodeDoc->UpdateDirectory( $iDirectoryID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		
		// Fill files
		$arrFiles = $hCodeDoc->ListFiles( $iDirectoryID, true );
		$iFileCount = count( $arrFiles );
		for( $i = 0; $i < $iFileCount; ++$i ) {
			$arrFileRow = $arrFiles[$i];
			$iFileID = $arrFileRow["FileID"];
			$strFileName = $arrFileRow["Name"];
			$this->_DocData_Import_File( $iFileID, $strFileName, $strContentPath . "/$strName" );
		}
		
		// Recurse
		$arrDirectories = $hCodeDoc->ListDirectories( $iDirectoryID, true );
		$iDirectoryCount = count( $arrDirectories );
		for( $i = 0; $i < $iDirectoryCount; ++$i ) {
			$arrDirectoryRow = $arrDirectories[$i];
			$iSubDirID = $arrDirectoryRow["DirectoryID"];
			$strSubDirName = $arrDirectoryRow["Name"];
			$this->_DocData_Import_Directory( $iSubDirID, $strSubDirName, $strContentPath . "/$strName" );
		}
	}
	private function _DocData_Import_File( $iFileID, $strName, $strContentPath )
	{
		$hXML = XMLManager::GetSingleton();
		$hCodeDoc = CodeDocManager::GetSingleton();

		// Open XML file
		$hDocument = $hXML->CreateDocument();
		$bOk = $hDocument->LoadFromFile( $strContentPath . "/". str_replace(".h", ".xml", $strName) );
		if ( !$bOk )
			return;
		$hRoot = $hDocument->GetRoot();
		
		// Fill file data
		$arrChildren = $hRoot->GetChildrenByTag( "file" );
		assert( 'count($arrChildren) == 1' );
		$hFile = $arrChildren[0];
		
		$arrDocData = $this->_DocData_Get( $hFile, "en" );
		$hCodeDoc->UpdateFile( $iFileID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		
		// Fill defines
		$arrChildren = $hRoot->GetChildrenByTag( "define" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hDefine = $arrChildren[$i];
			$strScopedNameID = $hDefine->GetAttributeValue( "id" );			
			$iDefineID = $hCodeDoc->GetDefineID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hDefine, "en" );
			$hCodeDoc->UpdateDefine( $iDefineID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
	
		// Fill macros
		$arrChildren = $hRoot->GetChildrenByTag( "macro" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hMacro = $arrChildren[$i];
			$strScopedNameID = $hMacro->GetAttributeValue( "id" );			
			$iMacroID = $hCodeDoc->GetMacroID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hMacro, "en" );
			$hCodeDoc->UpdateMacro( $iMacroID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}

		// Fill namespaces
		$arrChildren = $hRoot->GetChildrenByTag( "namespace" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hNamespace = $arrChildren[$i];
			$strScopedNameID = $hNamespace->GetAttributeValue( "id" );			
			$iNamespaceID = $hCodeDoc->GetNamespaceID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hNamespace, "en" );
			$hCodeDoc->UpdateNamespace( $iNamespaceID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
		
		// Fill types
		$arrChildren = $hRoot->GetChildrenByTag( "type" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hType = $arrChildren[$i];
			$strScopedNameID = $hType->GetAttributeValue( "id" );			
			$iTypeID = $hCodeDoc->GetTypeID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hType, "en" );
			$hCodeDoc->UpdateType( $iTypeID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
		
		// Fill enums
		$arrChildren = $hRoot->GetChildrenByTag( "enum" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hEnum = $arrChildren[$i];
			$strScopedNameID = $hEnum->GetAttributeValue( "id" );			
			$iEnumID = $hCodeDoc->GetEnumID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hEnum, "en" );
			$hCodeDoc->UpdateEnum( $iEnumID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
		
		// Fill typedefs
		$arrChildren = $hRoot->GetChildrenByTag( "typedef" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hTypedef = $arrChildren[$i];
			$strScopedNameID = $hTypedef->GetAttributeValue( "id" );			
			$iTypedefID = $hCodeDoc->GetTypedefID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hTypedef, "en" );
			$hCodeDoc->UpdateTypedef( $iTypedefID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
		
		// Fill variables
		$arrChildren = $hRoot->GetChildrenByTag( "variable" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hVariable = $arrChildren[$i];
			$strScopedNameID = $hVariable->GetAttributeValue( "id" );			
			$iVariableID = $hCodeDoc->GetVariableID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hVariable, "en" );
			$hCodeDoc->UpdateVariable( $iVariableID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
		
		// Fill functions
		$arrChildren = $hRoot->GetChildrenByTag( "function" );
		$iCount = count( $arrChildren );
		for( $i = 0; $i < $iCount; ++$i ) {
			$hFunction = $arrChildren[$i];
			$strScopedNameID = $hFunction->GetAttributeValue( "id" );			
			$iFunctionID = $hCodeDoc->GetFunctionID( $strScopedNameID );

			$arrDocData = $this->_DocData_Get( $hFunction, "en" );
			$hCodeDoc->UpdateFunction( $iFunctionID, $arrDocData[0]->GetTextContent(), $arrDocData[1]->GetTextContent() );
		}
	}
	
	private function _DocData_Export()
	{
		assert( 'false' );
/////////////////////////////////////////////////
	}

	// Members
	private $m_arrFileStructure;
	private $m_arrNamespaceStructure;
	private $m_arrTypeStructure;
	private $m_arrUnresolvedXReferences;
}

?>