<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLGUI.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : GUI Tags (non-input ones)
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
	// Menu types
define( "HTML_MENU_LIST",    "list" );    // Simple
define( "HTML_MENU_TOOLBAR", "toolbar" ); // Toolbar
define( "HTML_MENU_CONTEXT", "context" ); // Drop-down

/////////////////////////////////////////////////////////////////////////////////
// The HTMLMenu class
class HTMLMenu extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strType = NULL, $strLabel = NULL )
    {
		parent::__construct( "menu", $hParent );
		
		$this->m_strType = $strType;
		$this->m_strLabel = $strLabel;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_strType != NULL )
			$strAttributes .= " type=\"" . $this->m_strType . "\"";
		if ( $this->m_strLabel != NULL )
			$strAttributes .= " label=\"" . $this->m_strLabel . "\"";
	}

	// Members
	private $m_strType;
	private $m_strLabel;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLMenuItem class
class HTMLMenuItem extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iValue = NULL )
    {
		parent::__construct( "li", $hParent );
		
		$this->m_iValue = $iValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_iValue != NULL )
			$strAttributes .= " value=\"" . $this->m_iValue . "\"";
	}

	// Members
	private $m_iValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLRatingBar class
class HTMLRatingBar extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iMin, $iMax, $iValue, $iLow = NULL, $iHigh = NULL, $iBest = NULL )
    {
		parent::__construct( "meter", $hParent );
		
		$this->m_iMin = $iMin;
		$this->m_iMax = $iMax;
		$this->m_iValue = $iValue;
		$this->m_iLow = $iLow;
		$this->m_iHigh = $iHigh;
		$this->m_iBest = $iBest;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " min=\"" . $this->m_iMin . "\"";
		$strAttributes .= " max=\"" . $this->m_iMax . "\"";
		$strAttributes .= " value=\"" . $this->m_iValue . "\"";
		if ( $this->m_iLow != NULL )
			$strAttributes .= " low=\"" . $this->m_iLow . "\"";
		if ( $this->m_iHigh != NULL )
			$strAttributes .= " high=\"" . $this->m_iHigh . "\"";
		if ( $this->m_iBest != NULL )
			$strAttributes .= " optimum=\"" . $this->m_iBest . "\"";
	}

	// Members
	private $m_iMin;
	private $m_iMax;
	private $m_iValue;
	private $m_iLow;
	private $m_iHigh;
	private $m_iBest;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLProgressBar class
class HTMLProgressBar extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iMax, $iValue = NULL )
    {
		parent::__construct( "progress", $hParent );
		
		$this->m_iMax = $iMax;
		$this->m_iValue = $iValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " max=\"" . $this->m_iMax . "\"";
		if ( $this->m_iValue != NULL )
			$strAttributes .= " value=\"" . $this->m_iValue . "\"";
	}

	// Members
	private $m_iMax;
	private $m_iValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLCommand class
class HTMLCommand extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strType, $strLabel, $strIconURL = NULL, $strRadioGroup = NULL, $bChecked = false, $bDisabled = false )
    {
		parent::__construct( "command", $hParent );
		
		$this->m_strType = $strType;
		$this->m_strLabel = $strLabel;
		$this->m_strIconURL = $strIconURL;
		$this->m_strRadioGroup = $strRadioGroup;
		$this->m_bChecked = $bChecked;
		$this->m_bDisabled = $bDisabled;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"" . $this->m_strType . "\"";
		$strAttributes .= " label=\"" . $this->m_strLabel . "\"";
		if ( $this->m_strIconURL != NULL )
			$strAttributes .= " icon=\"" . $this->m_strIconURL . "\"";
		if ( ($this->m_strType == HTML_COMMAND_TYPE_RADIO) && ($this->m_strRadioGroup != NULL) )
			$strAttributes .= " radiogroup=\"" . $this->m_strRadioGroup . "\"";
		if ( ($this->m_strType != HTML_COMMAND_TYPE_COMMAND) && $this->m_bChecked )
			$strAttributes .= " checked=\"checked\"";
		if ( $this->m_bDisabled )
			$strAttributes .= " disabled=\"disabled\"";
	}

	// Members
	private $m_strType;
	private $m_strLabel;
	private $m_strIconURL;
	private $m_strRadioGroup;
	private $m_bChecked;
	private $m_bDisabled;
}

?>