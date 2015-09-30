<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/XML/XMLElement.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML Element object
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
// The XMLElement class
class XMLElement extends XMLNode
{
	// Constructor / Destructor
    public function __construct( $hElement = NULL )
    {
		parent::__construct( $hElement );
		assert( '$this->m_hNode != NULL' );
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Tag name
	public function GetTagName() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->tagName;
	}

	// Nodes access
	public function GetElementsByTag( $strTag, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return new XMLNodeList( $this->m_hNode->getElementsByTagNameNS( $strNamespace, $strTag ) );
		else
			return new XMLNodeList( $this->m_hNode->getElementsByTagName( $strTag ) );
	}

	// Children access
	public function GetChildrenByTag( $strTag )
	{
		assert( '$this->m_hNode != NULL' );
		if ( !($this->HasChildren()) )
			return array();
		$arrChildren = array();
		$iChildCount = $this->GetChildCount();
		for( $i = 0; $i < $iChildCount; ++$i ) {
			$hChild = $this->GetChild( $i );
			if ( $hChild->GetType() == XML_NODE_ELEMENT ) {
				if ( $hChild->GetTagName() == $strTag )
					array_push( $arrChildren, $hChild );
			}
		}
		return $arrChildren;
	}
	public function GetChildByTag( $strTag )
	{
		assert( '$this->m_hNode != NULL' );
		if ( !($this->HasChildren()) )
			return NULL;
		$iChildCount = $this->GetChildCount();
		for( $i = 0; $i < $iChildCount; ++$i ) {
			$hChild = $this->GetChild( $i );
			if ( $hChild->GetType() == XML_NODE_ELEMENT ) {
				if ( $hChild->GetTagName() == $strTag )
					return $hChild;
			}
		}
		return NULL;
	}
	public function GetChildByPath( $strPath )
	{
		assert( '$this->m_hNode != NULL' );
		$arrPath = explode( "/", $strPath );
		$iCount = count( $arrPath );
		$hNode = $this;
		for( $i = 0; $i < $iCount; ++$i ) {
			if ( $hNode->GetType() != XML_NODE_ELEMENT )
				break;
			$strTag = $arrPath[$i];
			$hNode = $hNode->GetChildByTag( $strTag );
		}
		return $hNode;
	}

	// Attributes access
	public function HasAttributes() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->hasAttributes();
	}
	public function HasAttribute( $strName, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return $this->m_hNode->hasAttributeNS( $strNamespace, $strName );
		else
			return $this->m_hNode->hasAttribute( $strName );
	}

	public function GetAttributeNode( $strName, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return new XMLAttribute( $this->m_hNode->getAttributeNodeNS( $strNamespace, $strName ) );
		else
			return new XMLAttribute( $this->m_hNode->getAttributeNode( $strName ) );
	}
	public function GetAttributeValue( $strName, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return $this->m_hNode->getAttributeNS( $strNamespace, $strName );
		else
			return $this->m_hNode->getAttribute( $strName );
	}

	public function SetAttributeNode( $hAttribute ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->setAttributeNode( $hAttribute->m_hNode );
		//$this->m_hNode->setAttributeNodeNS( $hAttribute->m_hNode );
	}
	public function SetAttributeValue( $strName, $strValue, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			$this->m_hNode->setAttributeNS( $strNamespace, $strName, $strValue );
		else
			$this->m_hNode->setAttribute( $strName, $strValue );
	}

	public function RemoveAttributeNode( $hAttribute ) {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->removeAttributeNode( $hAttribute->m_hNode );
	}
	public function RemoveAttribute( $strName, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			return $this->m_hNode->removeAttributeNS( $strNamespace, $strName );
		else
			return $this->m_hNode->removeAttribute( $strName );
	}

	// ID attributes
	public function SetIDAttributeNode( $hAttribute, $bIsID ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->setIdAttributeNode( $hAttribute->m_hNode, $bIsID );
	}
	public function SetIDAttribute( $strName, $bIsID, $strNamespace = NULL ) {
		assert( '$this->m_hNode != NULL' );
		if ( $strNamespace != NULL )
			$this->m_hNode->setIdAttributeNS( $strNamespace, $strName, $bIsID );
		else
			$this->m_hNode->setIdAttribute( $strName, $bIsID );
	}

	// Members
}

?>