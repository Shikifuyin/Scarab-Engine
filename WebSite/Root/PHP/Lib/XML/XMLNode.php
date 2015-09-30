<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/XML/XMLNode.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML Node interface
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
	// Node types
define( "XML_NODE_DOCUMENT", 	  XML_DOCUMENT_NODE );
define( "XML_NODE_ELEMENT", 	  XML_ELEMENT_NODE );
define( "XML_NODE_ATTRIBUTE", 	  XML_ATTRIBUTE_NODE );
define( "XML_NODE_COMMENT", 	  XML_COMMENT_NODE );
define( "XML_NODE_TEXT", 		  XML_TEXT_NODE );
define( "XML_NODE_TEXT_UNPARSED", XML_CDATA_SECTION_NODE );

/////////////////////////////////////////////////////////////////////////////////
// The XMLNode class
class XMLNode
{
	// Constructor / Destructor
    public function __construct( $hNode = NULL )
    {
		$this->m_hNode = $hNode;
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Operator
	public function TypeCast() {
		return $this->_MakeNode( $this->m_hNode );
	}
	public function Equals( $hNode ) {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->isSameNode( $hNode->m_hNode );
	}

	// Properties access
	public function GetType() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->nodeType;
	}
	public function GetName( $bNamespace = false ) {
		assert( '$this->m_hNode != NULL' );
		if ( $bNamespace )
			return array( "BaseURI" => $this->m_hNode->baseURI,
						  "NamespaceURI" => $this->m_hNode->namespaceURI,
						  "LocalName" => $this->m_hNode->localName );
		else
			return $this->m_hNode->nodeName;
	}
	public function GetValue() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->nodeValue;
	}
	public function GetTextContent() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->textContent;
	}

	public function GetLineNumber() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->getLineNo();
	}

	// Linkage
	public function GetOwnerDocument() {
		assert( '$this->m_hNode != NULL' );
		return new XMLDocument( $this->m_hNode->ownerDocument );
	}
	public function GetPath() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->getNodePath();
	}

	public function GetParent() {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->parentNode );
	}
	public function GetFirstChild() {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->firstChild );
	}
	public function GetLastChild() {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->lastChild );
	}
	public function GetPrevSibbling() {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->previousSibbling );
	}
	public function GetNextSibbling() {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->nextSibbling );
	}

	public function HasChildren() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->hasChildNodes();
	}
	public function GetChildCount() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->childNodes->length;
	}
	public function GetChild( $iIndex ) {
		assert( '$this->m_hNode != NULL' );
		assert( '$iIndex >= 0' );
		assert( '$iIndex < $this->m_hNode->childNodes->length' );
		return $this->_MakeNode( $this->m_hNode->childNodes->item( $iIndex ) );
	}

	// Edition
	public function AppendChild( $hNode ) {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->appendChild( $hNode->m_hNode ) );
	}
	public function InsertChild( $hNode, $hNewNode ) {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->insertBefore( $hNewNode->m_hNode, $hNode->m_hNode ) );
	}
	public function ReplaceChild( $hNode, $hNewNode ) {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->replaceChild( $hNewNode->m_hNode, $hNode->m_hNode ) );
	}
	public function RemoveChild( $hNode ) {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->removeChild( $hNode->m_hNode ) );
	}

	public function CloneNode( $bRecursive = true ) {
		assert( '$this->m_hNode != NULL' );
		return $this->_MakeNode( $this->m_hNode->cloneNode( $bRecursive ) );
	}

	// Normalization
	public function Normalize() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->normalize();
	}

	// Helpers
	protected function _MakeNode( $hNode ) {
		switch( $hNode->nodeType ) {
			case XML_NODE_DOCUMENT: 	 return new XMLDocument( $hNode );
			case XML_NODE_ELEMENT: 		 return new XMLElement( $hNode );
			case XML_NODE_ATTRIBUTE: 	 return new XMLAttribute( $hNode );
			case XML_NODE_COMMENT: 		 return new XMLComment( $hNode );
			case XML_NODE_TEXT: 		 return new XMLText( $hNode );
			case XML_NODE_TEXT_UNPARSED: return new XMLTextUnparsed( $hNode );
			default: 					 return new XMLNode( $hNode );
		}
	}

	// Members
	protected $m_hNode;
}

/////////////////////////////////////////////////////////////////////////////////
// The XMLNodeList class
class XMLNodeList
{
	// Constructor / Destructor
    public function __construct( $hNodeList = NULL )
    {
		$this->m_hNodeList = $hNodeList;
	}
	public function __destruct()
    {
		// nothing to do
	}

	public function GetCount() {
		assert( '$this->m_hNodeList != NULL' );
		return $this->m_hNodeList->length;
	}
	public function GetNode( $iIndex, $bTypeCast = true ) {
		assert( '$this->m_hNodeList != NULL' );
		assert( '$iIndex >= 0' );
		assert( '$iIndex < $this->m_hNodeList->length' );
		$hNode = new XMLNode( $this->m_hNodeList->item( $iIndex ) );
		return $bTypeCast ? $hNode->TypeCast() : $hNode;
	}

	// Members
	private $m_hNodeList;
}

?>