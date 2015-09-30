<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLNode.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : Node Tags
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
// The HTMLNode class
class HTMLNode extends HTMLTag
{
	// Constructor / Destructor
    public function __construct( $strTag, $hParent, $bAllowAttributes = true )
    {
		parent::__construct( $strTag, $hParent, $bAllowAttributes );
		
		$this->m_arrChildren = array();
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Children access
	public function GetChildren() {
		return $this->m_arrChildren;
	}
	public function GetChildCount() {
		return count( $this->m_arrChildren );
	}
	public function GetChild( $iIndex ) {
		return $this->m_arrChildren[$iIndex];
	}
	public function GetChildByID( $strID ) {
		foreach( $this->m_arrChildren as $hChild ) {
			if ( $hChild->GetID() == $strID )
				return $hChild;
		}
	}
	public function GetChildrenByTag( $strTag ) {
		$arrChildren = array();
		foreach( $this->m_arrChildren as $hChild ) {
			if ( $hChild->GetTag() == $strTag )
				array_push( $arrChildren, $hChild );
		}
		return $arrChildren;
	}

	// Tree Operations
	public function PrependChild( $hChild ) {
		$hChild->m_hParent = $this;
		array_unshift( $this->m_arrChildren, $hChild );
	}
	public function AppendChild( $hChild ) {
		$hChild->m_hParent = $this;
		array_push( $this->m_arrChildren, $hChild );
	}
	public function ReplaceChild( $iAt, $hChild ) {
		$this->m_arrChildren[$iAt]->m_hParent = NULL;
		$hChild->m_hParent = $this;
		$this->m_arrChildren[$iAt] = $hChild;
	}
	public function InsertChild( $iAt, $hChild ) {
		$hChild->m_hParent = $this;
		array_splice( $this->m_arrChildren, $iAt, 0, array( $hChild ) );
	}
	public function RemoveChild( $iAt ) {
		$this->m_arrChildren[$iAt]->m_hParent = NULL;
		array_splice( $this->m_arrChildren, $iAt, 1 );
	}
	public function RemoveAllChildren() {
		foreach( $this->m_arrChildren as $hChild )
			$hChild->m_hParent = NULL;
		$this->m_arrChildren = array();
	}
	public function SpliceChildren( $iDeleteAt, $iDeleteCount, $arrInsertChildren ) {
		$iDeleteEnd = $iDeleteAt + $iDeleteCount;
		for( $i = $iDeleteAt; $i < $iDeleteEnd; ++$i )
			$this->m_arrChildren[$i]->m_hParent = NULL;
		if ( $arrInsertChildren != NULL ) {
			foreach( $arrInsertChildren as $hChild )
				$hChild->m_hParent = $this;
		}
		array_splice( $this->m_arrChildren, $iDeleteAt, $iDeleteCount, $arrInsertChildren );
	}

	// Output routine
	public function Output( $iIndentDepth = 0 ) {
		$strIndent = $this->_GetIndentString( $iIndentDepth );
		$this->_Recursive_Output( $strIndent );
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		// default : do nothing
	}
	protected function _Output_BeforeContent_Callback() {
		// default : do nothing
		return NULL;
	}
	protected function _Output_AfterContent_Callback() {
		// default : do nothing
		return NULL;
	}

	protected function _Recursive_Output( $strIndent )
	{
		// Open node
		$strOpenTag = "<" . $this->m_strTag;
		if ( $this->m_bAllowAttributes )
			$this->_ApplyGlobalAttributes( $strOpenTag );
		$this->_Output_Attributes_Callback( $strOpenTag );
		$strOpenTag .= " >";
		echo "$strIndent$strOpenTag";
		
		// Content
		$strContentIndent = $strIndent . "\t";

		$strBefore = $this->_Output_BeforeContent_Callback();
		if ( $strBefore != NULL )
			echo "$strContentIndent$strBefore";

		foreach( $this->m_arrChildren as $hChild )
			$hChild->_Recursive_Output( $strContentIndent );

		$strAfter = $this->_Output_AfterContent_Callback();
		if ( $strAfter != NULL )
			echo "$strContentIndent$strAfter";

		// Close node
		$strCloseTag = "</" . $this->m_strTag . ">";
		echo "$strIndent$strCloseTag";
	}

	// Members
	protected $m_arrChildren;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputNode class
class HTMLInputNode extends HTMLInputTag
{
	// Constructor / Destructor
    public function __construct( $strTag, $strInputType, $hParent, $bAllowAttributes = true )
    {
		parent::__construct( $strTag, $strInputTyp, $hParente, $bAllowAttributes );
		
		$this->m_arrChildren = array();
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Children access
	public function GetChildren() {
		return $this->m_arrChildren;
	}
	public function GetChildCount() {
		return count( $this->m_arrChildren );
	}
	public function GetChild( $iIndex ) {
		return $this->m_arrChildren[$iIndex];
	}
	public function GetChildByID( $strID ) {
		foreach( $this->m_arrChildren as $hChild ) {
			if ( $hChild->GetID() == $strID )
				return $hChild;
		}
	}
	public function GetChildrenByTag( $strTag ) {
		$arrChildren = array();
		foreach( $this->m_arrChildren as $hChild ) {
			if ( $hChild->GetTag() == $strTag )
				array_push( $arrChildren, $hChild );
		}
		return $arrChildren;
	}

	// Tree Operations
	public function PrependChild( $hChild ) {
		$hChild->m_hParent = $this;
		array_unshift( $this->m_arrChildren, $hChild );
	}
	public function AppendChild( $hChild ) {
		$hChild->m_hParent = $this;
		array_push( $this->m_arrChildren, $hChild );
	}
	public function ReplaceChild( $iAt, $hChild ) {
		$this->m_arrChildren[$iAt]->m_hParent = NULL;
		$hChild->m_hParent = $this;
		$this->m_arrChildren[$iAt] = $hChild;
	}
	public function InsertChild( $iAt, $hChild ) {
		$hChild->m_hParent = $this;
		array_splice( $this->m_arrChildren, $iAt, 0, array( $hChild ) );
	}
	public function RemoveChild( $iAt ) {
		$this->m_arrChildren[$iAt]->m_hParent = NULL;
		array_splice( $this->m_arrChildren, $iAt, 1 );
	}
	public function RemoveAllChildren() {
		foreach( $this->m_arrChildren as $hChild )
			$hChild->m_hParent = NULL;
		$this->m_arrChildren = array();
	}
	public function SpliceChildren( $iDeleteAt, $iDeleteCount, $arrInsertChildren ) {
		$iDeleteEnd = $iDeleteAt + $iDeleteCount;
		for( $i = $iDeleteAt; $i < $iDeleteEnd; ++$i )
			$this->m_arrChildren[$i]->m_hParent = NULL;
		if ( $arrInsertChildren != NULL ) {
			foreach( $arrInsertChildren as $hChild )
				$hChild->m_hParent = $this;
		}
		array_splice( $this->m_arrChildren, $iDeleteAt, $iDeleteCount, $arrInsertChildren );
	}

	// Output routine
	public function Output( $iIndentDepth = 0 ) {
		$strIndent = $this->_GetIndentString( $iIndentDepth );
		$this->_Recursive_Output( $strIndent );
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		// default : do nothing
	}
	protected function _Output_BeforeContent_Callback() {
		// default : do nothing
		return NULL;
	}
	protected function _Output_AfterContent_Callback() {
		// default : do nothing
		return NULL;
	}

	protected function _Recursive_Output( $strIndent )
	{
		// Open node
		$strOpenTag = "<" . $this->m_strTag;
		if ( $this->m_bAllowAttributes ) {
			$this->_ApplyGlobalAttributes( $strOpenTag );
			$this->_ApplyInputAttributes( $strOpenTag );
		}
		$this->_Output_Attributes_Callback( $strOpenTag );
		$strOpenTag .= " >";
		echo "$strIndent$strOpenTag";
		
		// Content
		$strContentIndent = $strIndent . "\t";

		$strBefore = $this->_Output_BeforeContent_Callback();
		if ( $strBefore != NULL )
			echo "$strContentIndent$strBefore";

		foreach( $this->m_arrChildren as $hChild )
			$hChild->_Recursive_Output( $strContentIndent );

		$strAfter = $this->_Output_AfterContent_Callback();
		if ( $strAfter != NULL )
			echo "$strContentIndent$strAfter";

		// Close node
		$strCloseTag = "</" . $this->m_strTag . ">";
		echo "$strIndent$strCloseTag";
	}

	// Members
	protected $m_arrChildren;
}

?>