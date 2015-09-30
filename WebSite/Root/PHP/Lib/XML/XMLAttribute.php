<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/XML/XMLAttribute.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML Attribute object
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
// The XMLAttribute class
class XMLAttribute extends XMLNode
{
	// Constructor / Destructor
    public function __construct( $hAttribute = NULL )
    {
		parent::__construct( $hAttribute );
		assert( '$this->m_hNode != NULL' );
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Attribute access
	public function GetOwnerElement() {
		assert( '$this->m_hNode != NULL' );
		return new XMLElement( $this->m_hNode->ownerElement );
	}

	public function IsID() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->isId();
	}
	public function GetAttributeName() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->name;
	}
	public function GetValue() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->value;
	}
	public function SetValue( $strValue ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->value = $strValue;
	}

	// Members
}

?>