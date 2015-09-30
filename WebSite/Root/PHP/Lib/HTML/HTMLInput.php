<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLInput.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : GUI Input Tags
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
	// Form's methods
define( "HTML_FORM_METHOD_GET",  HTTP_METHOD_GET );
define( "HTML_FORM_METHOD_POST", HTTP_METHOD_POST );

	// Form's encoding types
define( "HTML_FORM_ENCODING_URL",    MIME_APPLICATION_FORM_URL );
define( "HTML_FORM_ENCODING_SPACES", MIME_TEXT_PLAIN );
define( "HTML_FORM_ENCODING_NONE",   MIME_MULTIPART_FORM );

	// Form's keygen types
define( "HTML_FORM_KEYGEN_RSA", "rsa" );
define( "HTML_FORM_KEYGEN_DSA", "dsa" );
define( "HTML_FORM_KEYGEN_EC",  "ec" );

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The HTMLForm class
class HTMLForm extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName, $strAction, $strMethod = NULL, $strTarget = NULL,
							   			   $strEncType = NULL, $strCharsets = NULL, $bAutoComplete = NULL, $bNoValidation = false )
    {
		parent::__construct( "form", $hParent );
		
		$this->m_strName = $strName;
		$this->m_strAction = $strAction;
		$this->m_strMethod = $strMethod;
		$this->m_strTarget = $strTarget;
		$this->m_strEncType = $strEncType;
		$this->m_strCharsets = $strCharsets;
		$this->m_bAutoComplete = $bAutoComplete;
		$this->m_bNoValidation = $bNoValidation;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
		$strAttributes .= " action=\"" . $this->m_strAction . "\"";
		if ( $this->m_strMethod != NULL )
			$strAttributes .= " method=\"" . $this->m_strMethod . "\"";
		if ( $this->m_strTarget != NULL )
			$strAttributes .= " target=\"" . $this->m_strTarget . "\"";
		if ( $this->m_strEncType != NULL )
			$strAttributes .= " enctype=\"" . $this->m_strEncType . "\"";
		if ( $this->m_strCharsets != NULL )
			$strAttributes .= " accept-charset=\"" . $this->m_strCharsets . "\"";
		if ( $this->m_bAutoComplete != NULL ) {
			$strOnOff = ($this->m_bAutoComplete) ? "on" : "off";
			$strAttributes .= " autocomplete=\"$strOnOff\"";
		}
		if ( $this->m_bNoValidation )
			$strAttributes .= " novalidate=\"novalidate\"";
	}

	// Members
	private $m_strName;
	private $m_strAction;
	private $m_strMethod;
	private $m_strTarget;
	private $m_strEncType;
	private $m_strCharsets;
	private $m_bAutoComplete;
	private $m_bNoValidation;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFormBlock class
class HTMLFormBlock extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName, $bDisabled = false )
    {
		parent::__construct( "fieldset", $hParent );
		
		$this->m_strName = $strName;
		$this->m_bDisabled = $bDisabled;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
		if ( $this->m_bDisabled )
			$strAttributes .= " disabled=\"disabled\"";
	}

	// Members
	private $m_strName;
	private $m_bDisabled;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFormBlockLegend class
class HTMLFormBlockLegend extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "legend", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFormOutput class
class HTMLFormOutput extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName, $strInputIDs )
    {
		parent::__construct( "output", $hParent );

		$this->m_strName = $strName;
		$this->m_strInputIDs = $strInputIDs;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
		$strAttributes .= " for=\"" . $this->m_strInputIDs . "\"";
	}

	// Members
	private $m_strName;
	private $m_strInputIDs;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFormResetButton class
class HTMLFormResetButton extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strLabel = "Reset" )
    {
		parent::__construct( "input", "reset", $hParent );

		$this->m_strLabel = $strLabel;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"reset\"";
		$strAttributes .= " value=\"" . $this->m_strLabel . "\"";
	}

	// Members
	private $m_strLabel;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFormSubmitButton class
class HTMLFormSubmitButton extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strLabel = "Submit" )
    {
		parent::__construct( "input", "submit", $hParent );

		$this->m_strLabel = $strLabel;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"submit\"";
		$strAttributes .= " value=\"" . $this->m_strLabel . "\"";
	}

	// Members
	private $m_strLabel;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFormSubmitImage class
class HTMLFormSubmitImage extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strSourceURL, $iWidth, $iHeigh, $strAltText = "Submit", $strValue = NULL )
    {
		parent::__construct( "input", "image", $hParent );

		$this->m_strSourceURL = $strSourceURL;
		$this->m_iWidth = $iWidth;
		$this->m_iHeigh = $iHeigh;
		$this->m_strAltText = $strAltText;
		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"image\"";
		$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		$strAttributes .= " height=\"" . $this->m_iHeigh . "\"";
		$strAttributes .= " alt=\"" . $this->m_strAltText . "\"";
		if ( $this->m_strValue != NULL )
            $strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strSourceURL;
	private $m_iWidth;
	private $m_iHeigh;
	private $m_strAltText;
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputLabel class
class HTMLInputLabel extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strTargetID )
    {
		parent::__construct( "label", $hParent );

		$this->m_strTargetID = $strTargetID;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " for=\"" . $this->m_strTargetID . "\"";
	}

	// Members
	private $m_strTargetID;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputDataList class
class HTMLInputDataList extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "datalist", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputDataListItem class
class HTMLInputDataListItem extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL, $strLabel = NULL, $bSelected = false, $bDisabled = false )
    {
		parent::__construct( "option", $hParent );

		$this->m_strValue = $strValue;
		$this->m_strLabel = $strLabel;
		$this->m_bSelected = $bSelected;
		$this->m_bDisabled = $bDisabled;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_strValue != NULL )
            $strAttributes .= " value=\"" . $this->m_strValue . "\"";
		if ( $this->m_strLabel != NULL )
            $strAttributes .= " label=\"" . $this->m_strLabel . "\"";
		if ( $this->m_bSelected )
            $strAttributes .= " selected=\"selected\"";
		if ( $this->m_bDisabled )
            $strAttributes .= " disabled=\"disabled\"";
	}

	// Members
	private $m_strValue;
	private $m_strLabel;
	private $m_bSelected;
	private $m_bDisabled;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputHidden class
class HTMLInputHidden extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue )
    {
		parent::__construct( "input", "hidden", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"hidden\"";
		$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputNumber class
class HTMLInputNumber extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL, $bEstimation = false )
    {
		parent::__construct( "input", $bEstimation ? "range" : "number", $hParent );

		$this->m_strValue = $strValue;
		$this->m_bEstimation = $bEstimation;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strType = ($this->m_bEstimation) ? "range" : "number";
		$strAttributes .= " type=\"$strType\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
	private $m_bEstimation;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputColor class
class HTMLInputColor extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "color", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"color\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputText class
class HTMLInputText extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "text", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"text\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputPassword class
class HTMLInputPassword extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent /*, $strValue = NULL*/ )
    {
		parent::__construct( "input", "password", $hParent );

		//$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"password\"";
		//if ( $this->m_strValue != NULL )
		//    $strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	//private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputEMail class
class HTMLInputEMail extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL, $bMultiple = false )
    {
		parent::__construct( "input", "email", $hParent );

		$this->m_strValue = $strValue;
		$this->m_bMultiple = $bMultiple;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"email\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
		if ( $this->m_bMultiple )
			$strAttributes .= " multiple=\"multiple\"";
	}

	// Members
	private $m_strValue;
	private $m_bMultiple;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputPhone class
class HTMLInputPhone extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "tel", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"tel\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputURL class
class HTMLInputURL extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "url", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"url\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputSearch class
class HTMLInputSearch extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "search", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"search\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputDate class
class HTMLInputDate extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "date", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"date\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputTime class
class HTMLInputTime extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "time", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"time\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputDateTime class
class HTMLInputDateTime extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL, $bLocal = false )
    {
		parent::__construct( "input", $bLocal ? "datetime-local" : "datetime", $hParent );

		$this->m_strValue = $strValue;
		$this->m_bLocal = $bLocal;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strType = ($this->m_bLocal) ? "datetime-local" : "datetime";
		$strAttributes .= " type=\"$strType\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
	private $m_bLocal;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputMonth class
class HTMLInputMonth extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "month", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"month\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputWeek class
class HTMLInputWeek extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL )
    {
		parent::__construct( "input", "week", $hParent );

		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"week\"";
		if ( $this->m_strValue != NULL )
			$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputTextArea class
class HTMLInputTextArea extends HTMLInputNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iRowCount = 4, $iColumnCount = 24, $bWrapText = NULL )
    {
		parent::__construct( "textarea", "textarea", $hParent );

		$this->m_iRowCount = $iRowCount;
		$this->m_iColumnCount = $iColumnCount;
		$this->m_bWrapText = $bWrapText;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " rows=\"" . $this->m_iRowCount . "\"";
		$strAttributes .= " cols=\"" . $this->m_iColumnCount . "\"";
		if ( $this->m_bWrapText != NULL ) {
			$strWrap = ($this->m_bWrapText) ? "hard" : "soft";
			$strAttributes .= " wrap=\"$strWrap\"";
		}
	}

	// Members
	private $m_iRowCount;
	private $m_iColumnCount;
	private $m_bWrapText;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputFile class
class HTMLInputFile extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strTypes = NULL, $bMultiple = false )
    {
		parent::__construct( "input", "file", $hParent );

		$this->m_strTypes = $strTypes;
		$this->m_bMultiple = $bMultiple;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"file\"";
		if ( $this->m_strTypes != NULL )
			$strAttributes .= " accept=\"" . $this->m_strTypes . "\"";
		if ( $this->m_bMultiple )
			$strAttributes .= " multiple=\"multiple\"";
	}

	// Members
	private $m_strTypes;
	private $m_bMultiple;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputButton class
class HTMLInputButton extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strLabel = "Button" )
    {
		parent::__construct( "input", "button", $hParent );

		$this->m_strLabel = $strLabel;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"button\"";
		$strAttributes .= " value=\"" . $this->m_strLabel . "\"";
	}

	// Members
	private $m_strLabel;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputCheckBox class
class HTMLInputCheckBox extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL, $bChecked = false )
    {
		parent::__construct( "input", "checkbox", $hParent );

		$this->m_strValue = $strValue;
		$this->m_bChecked = $bChecked;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"checkbox\"";
		if ( $this->m_strValue != NULL )
            $strAttributes .= " value=\"" . $this->m_strValue . "\"";
		if ( $this->m_bChecked )
            $strAttributes .= " checked=\"checked\"";
	}

	// Members
	private $m_strValue;
	private $m_bChecked;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputRadioButton class
class HTMLInputRadioButton extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strValue = NULL, $bChecked = false )
    {
		parent::__construct( "input", "radio", $hParent );

		$this->m_strValue = $strValue;
		$this->m_bChecked = $bChecked;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"radio\"";
		if ( $this->m_strValue != NULL )
            $strAttributes .= " value=\"" . $this->m_strValue . "\"";
		if ( $this->m_bChecked )
            $strAttributes .= " checked=\"checked\"";
	}

	// Members
	private $m_strValue;
	private $m_bChecked;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputKeygen class
class HTMLInputKeygen extends HTMLInputLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strKeyType = NULL )
    {
		parent::__construct( "keygen", "keygen", $hParent );

		$this->m_strKeyType = $strKeyType;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_strKeyType != NULL )
            $strAttributes .= " keytype=\"" . $this->m_strKeyType . "\"";
	}

	// Members
	private $m_strKeyType;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputComboList class
class HTMLInputComboList extends HTMLInputNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iListSize = NULL, $bMultiple = false )
    {
		parent::__construct( "select", "select", $hParent );

		$this->m_iListSize = $iListSize;
		$this->m_bMultiple = $bMultiple;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_iListSize != NULL )
            $strAttributes .= " size=\"" . $this->m_iListSize . "\"";
		if ( $this->m_bMultiple )
            $strAttributes .= " multiple=\"multiple\"";
	}

	// Members
	private $m_iListSize;
	private $m_bMultiple;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLComboListGroup class
class HTMLComboListGroup extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strLabel, $bDisabled = false )
    {
		parent::__construct( "optgroup", $hParent );

		$this->m_strLabel = $strLabel;
		$this->m_bDisabled = $bDisabled;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " label=\"" . $this->m_strLabel . "\"";
		if ( $this->m_bDisabled )
            $strAttributes .= " disabled=\"disabled\"";
	}

	// Members
	private $m_strLabel;
	private $m_bDisabled;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLComboListItem class
class HTMLComboListItem extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strLabel, $strValue = NULL, $bSelected = false, $bDisabled = false )
    {
		parent::__construct( "option", $hParent );

		$this->m_strLabel = $strLabel;
		$this->m_strValue = $strValue;
		$this->m_bSelected = $bSelected;
		$this->m_bDisabled = $bDisabled;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " label=\"" . $this->m_strLabel . "\"";
		if ( $this->m_strValue != NULL )
            $strAttributes .= " value=\"" . $this->m_strValue . "\"";
		if ( $this->m_bSelected )
            $strAttributes .= " selected=\"selected\"";
		if ( $this->m_bDisabled )
            $strAttributes .= " disabled=\"disabled\"";
	}

	// Members
	private $m_strLabel;
	private $m_strValue;
	private $m_bSelected;
	private $m_bDisabled;
}

?>