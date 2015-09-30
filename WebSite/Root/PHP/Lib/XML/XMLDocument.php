<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/XML/XMLDocument.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML Document object
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The XMLDocument class
class XMLDocument extends XMLNode
{
	// Constructor / Destructor
    public function __construct( $hDocument = NULL )
    {
		parent::__construct( $hDocument );
		assert( '$this->m_hNode != NULL' );

		$this->m_hNode->validateOnParse = false;
		$this->m_hNode->formatOutput = true;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Properties access
//	public function GetDocType() {
//		assert( '$this->m_hNode != NULL' );
//		return new XMLDocType( $this->m_hNode->doctype );
//	}
	public function GetURI() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->documentURI;
	}
	public function GetVersion() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->xmlVersion;
	}
	public function GetEncoding() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->xmlEncoding;
	}

	// Nodes access
	public function GetRoot() {
		assert( '$this->m_hNode != NULL' );
		return new XMLElement( $this->m_hNode->documentElement );
	}

	public function GetElementByID( $strNodeID ) {
		assert( '$this->m_hNode != NULL' );
		return new XMLElement( $this->m_hNode->getElementById( $strNodeID ) );
	}
	public function GetElementsByTag( $strTag, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return new XMLNodeList( $this->m_hNode->getElementsByTagNameNS( $strNamespace, $strTag ) );
		else
			return new XMLNodeList( $this->m_hNode->getElementsByTagName( $strTag ) );
	}

	// Node creation
	public function CreateElement( $strName, $strValue = NULL, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return new XMLElement( $this->m_hNode->createElementNS( $strNamespace, $strName, $strValue ) );
		else
			return new XMLElement( $this->m_hNode->createElement( $strName, $strValue ) );
	}
	public function CreateComment( $strComment ) {
		assert( '$this->m_hNode != NULL' );
		return new XMLComment( $this->m_hNode->createComment( $strComment ) );
	}
	public function CreateText( $strText ) {
		assert( '$this->m_hNode != NULL' );
		return new XMLText( $this->m_hNode->createTextNode( $strText ) );
	}
	public function CreateTextUnparsed( $strText ) {
		assert( '$this->m_hNode != NULL' );
		return new XMLTextUnparsed( $this->m_hNode->createCDATASection( $strText ) );
	}
	public function CreateAttribute( $strName, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return new XMLAttribute( $this->m_hNode->createAttributeNS( $strNamespace, $strName ) );
		else
			return new XMLAttribute( $this->m_hNode->createAttribute( $strName ) );
	}

	// Load / Save
	public function LoadFromString( $strXML ) {
		assert( '$this->m_hNode != NULL' );
		$bOK = $this->m_hNode->loadXML( $strXML );
		assert( '$bOK' );
	}
	public function LoadFromFile( $strFilename ) {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->load( $strFilename );
	}

	public function SaveToString( $hNode = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $hNode != NULL )
			return $this->m_hNode->saveXML( $hNode->m_hNode );
		else
			return $this->m_hNode->saveXML();
	}
	public function SaveToFile( $strFilename ) {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->save( $strFilename );
	}

	// Includes & imports
	public function ReplaceIncludes() {
		assert( '$this->m_hNode != NULL' );
		$iCount = $this->m_hNode->xinclude();
		assert( '$iCount != -1' );
		return $iCount;
	}
	public function ImportNode( $hImportNode, $bRecursive = true ) {
		assert( '$this->m_hNode != NULL' );
		return _MakeNode( $this->m_hNode->importNode( $hImportNode->m_hNode, $bRecursive ) );
	}

	// Raw XML injection
	public function AppendXML( $hNode, $strXML ) {
		assert( '$this->m_hNode != NULL' );
		$hFragment = $this->m_hNode->createDocumentFragment();
		$bOK = $hFragment->appendXML( $strXML );

		assert( '$hNode->GetOwnerDocument().Equals( $this )' );
		return _MakeNode( $hNode->m_hNode->appendChild( $hFragment ) );
	}
	public function ReplaceXML( $hNode, $strXML ) {
		assert( '$this->m_hNode != NULL' );
		$hFragment = $this->m_hNode->createDocumentFragment();
		$bOK = $hFragment->appendXML( $strXML );

		assert( '$hNode->GetOwnerDocument().Equals( $this )' );
		return _MakeNode( $hNode->m_hNode->parentNode->replaceChild( $hFragment, $hNode->m_hNode ) );
	}

	// Validation & normalization
	public function ValidateDTD() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->validate();
	}
	public function ValidateSchema( $strSchema, $bIsFilename = true ) {
		assert( '$this->m_hNode != NULL' );
		if ( $bIsFilename )
			return $this->m_hNode->schemaValidate( $strSchema );
		else
			return $this->m_hNode->schemaValidateSource( $strSchema );
	}

	public function NormalizeDocument() {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->normalizeDocument();
	}

	// Members
}

?>