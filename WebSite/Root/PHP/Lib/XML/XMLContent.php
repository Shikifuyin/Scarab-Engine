<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/XML/XMLContent.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML Content objects
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
// The XMLStringData class
class XMLStringData extends XMLNode
{
	// Constructor / Destructor
    public function __construct( $hStringData = NULL )
    {
		parent::__construct( $hStringData );
		assert( '$this->m_hNode != NULL' );
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Data access
	public function GetDataLength() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->length;
	}
	public function GetData() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->data;
	}
	public function GetDataSubString( $iOffset, $iLength ) {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->substringData( $iOffset, $iLength );
	}

	public function SetData( $strData ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->data = $strData;
	}
	public function AppendData( $strData ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->appendData( $strData );
	}
	public function InsertData( $iOffset, $strData ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->insertData( $iOffset, $strData );
	}
	public function ReplaceData( $iOffset, $iLength, $strData ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->replaceData( $iOffset, $iLength, $strData );
	}
	public function DeleteData( $iOffset, $iLength ) {
		assert( '$this->m_hNode != NULL' );
		$this->m_hNode->deleteData( $iOffset, $iLength );
	}

	// Members
}

/////////////////////////////////////////////////////////////////////////////////
// The XMLComment class
class XMLComment extends XMLStringData
{
	// Constructor / Destructor
    public function __construct( $hComment = NULL )
    {
		parent::__construct( $hComment );
		assert( '$this->m_hNode != NULL' );
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Members
}

/////////////////////////////////////////////////////////////////////////////////
// The XMLText class
class XMLText extends XMLStringData
{
	// Constructor / Destructor
    public function __construct( $hText = NULL )
    {
		parent::__construct( $hText );
		assert( '$this->m_hNode != NULL' );
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Concatenation of all sibbling text nodes skipping Elements, Comments, etc ...
	public function GetWholeText() {
		assert( '$this->m_hNode != NULL' );
		return $this->m_hNode->wholeText;
	}

	// Extraction helper
	public function Split( $iIndex ) {
		assert( '$this->m_hNode != NULL' );
		return new XMLText( $this->m_hNode->splitText( $iIndex ) );
	}

	// Members
}

/////////////////////////////////////////////////////////////////////////////////
// The XMLTextUnparsed class
class XMLTextUnparsed extends XMLText
{
	// Constructor / Destructor
    public function __construct( $hUnparsedText = NULL )
    {
		parent::__construct( $hUnparsedText );
		assert( '$this->m_hNode != NULL' );
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Members
}

?>