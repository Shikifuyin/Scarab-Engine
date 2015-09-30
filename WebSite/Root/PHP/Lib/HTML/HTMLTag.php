<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLTag.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : HTMLTag base classes
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Remove generalized input attributes and go specific ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
	// Events
		// Keyboard
define( "HTML_EVENT_ONKEYDOWN",  "onkeydown" );
define( "HTML_EVENT_ONKEYUP",    "onkeyup" );
define( "HTML_EVENT_ONKEYPRESS", "onkeypress" );

		// Mouse
define( "HTML_EVENT_ONMOUSEDOWN",     "onmousedown" );
define( "HTML_EVENT_ONMOUSEUP",       "onmouseup" );
define( "HTML_EVENT_ONMOUSECLICK",    "onclick" );
define( "HTML_EVENT_ONMOUSEDBLCLICK", "ondblclick" );
define( "HTML_EVENT_ONMOUSEWHEEL",    "onmousewheel" );

define( "HTML_EVENT_ONMOUSEMOVE", "onmousemove" );
define( "HTML_EVENT_ONMOUSEOVER", "onmouseover" );
define( "HTML_EVENT_ONMOUSEOUT",  "onmouseout" );

define( "HTML_EVENT_ONDRAG",      "ondrag" );
define( "HTML_EVENT_ONDROP",      "ondrop" );
define( "HTML_EVENT_ONDRAGSTART", "ondragstart" );
define( "HTML_EVENT_ONDRAGEND",   "ondragend" );
define( "HTML_EVENT_ONDRAGENTER", "ondragenter" ); //
define( "HTML_EVENT_ONDRAGLEAVE", "ondragleave" ); // Rolling over valid drop target
define( "HTML_EVENT_ONDRAGOVER",  "ondragover" );  //

define( "HTML_EVENT_ONSCROLL", "onscroll" );

		// Window
define( "HTML_EVENT_BEFORELOAD", "onbeforeonload" );
define( "HTML_EVENT_ONLOAD",     "onload" );
define( "HTML_EVENT_ONUNLOAD",   "onunload" );
define( "HTML_EVENT_ONONLINE",   "ononline" );
define( "HTML_EVENT_ONOFFLINE",  "onoffline" );

define( "HTML_EVENT_ONMESSAGE", "onmessage" );
define( "HTML_EVENT_ONERROR",   "onerror" );

define( "HTML_EVENT_ONSHOW",      "onpageshow" );
define( "HTML_EVENT_ONHIDE",      "onpagehide" );
define( "HTML_EVENT_ONRESIZE",    "onresize" );

define( "HTML_EVENT_ONPAGECHANGE",    "onhaschange" );
define( "HTML_EVENT_ONHISTORYCHANGE", "onpopstate" );
define( "HTML_EVENT_ONUNDO",          "onundo" );
define( "HTML_EVENT_ONREDO",          "onredo" );

define( "HTML_EVENT_BEFOREPRINT", "onbeforeprint" );
define( "HTML_EVENT_AFTERPRINT",  "onafterprint" );
define( "HTML_EVENT_ONSTORAGE",   "onstorage" );

		// Elements
define( "HTML_EVENT_ONCHANGE", "onchange" );
define( "HTML_EVENT_ONINPUT",  "oninput" );
define( "HTML_EVENT_ONSUBMIT", "onsubmit" );

define( "HTML_EVENT_ONINVALID", "oninvalid" );
define( "HTML_EVENT_ONABORT",   "onabort" );

define( "HTML_EVENT_ONSELECT",    "onselect" );
define( "HTML_EVENT_ONFOCUSGAIN", "onfocus" );
define( "HTML_EVENT_ONFOCUSLOST", "onblur" );

define( "HTML_EVENT_ONCONTEXTMENU", "oncontextmenu" );

		// Forms
define( "HTML_EVENT_ONFORMCHANGE", "onformchange" );
define( "HTML_EVENT_ONFORMINPUT",  "onforminput" );

		// Medias
define( "HTML_EVENT_ONLOADSTART",    "onloadstart" );
define( "HTML_EVENT_ONLOADPROGRESS", "onprogress" );
define( "HTML_EVENT_ONMETALOADED",   "onloadedmetadata" );
define( "HTML_EVENT_ONDATALOADED",   "onloadeddata" );

define( "HTML_EVENT_ONBUFFERED",      "oncanplay" );
define( "HTML_EVENT_ONFULLYBUFFERED", "oncanplaythrough" );

define( "HTML_EVENT_ONDATAWAITING",     "onwaiting" );
define( "HTML_EVENT_ONDATAINTERRUPT",   "onsuspend" );
define( "HTML_EVENT_ONDATAUNAVAILABLE", "onstalled" );
define( "HTML_EVENT_ONFILEUNAVAILABLE", "onemptied" );

define( "HTML_EVENT_ONDATASTATECHANGE", "onreadystatechange" );
define( "HTML_EVENT_ONPLAYRATECHANGE",  "onratechange" );
define( "HTML_EVENT_ONDURATIONCHANGE",  "ondurationchange" );
define( "HTML_EVENT_ONVOLUMECHANGE",    "onvolumechange" );

define( "HTML_EVENT_ONMEDIAPLAY",  "onplay" );
define( "HTML_EVENT_ONMEDIAPAUSE", "onpause" );
define( "HTML_EVENT_ONMEDIAEND",   "onended" );

define( "HTML_EVENT_ONSEEKSTART", "onseeking" );
define( "HTML_EVENT_ONSEEKEND",   "onseeked" );

define( "HTML_EVENT_ONMEDIAPLAYING", "onplaying" );
define( "HTML_EVENT_ONTIMEUPDATE",   "ontimeupdate" );

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTag class
class HTMLTag
{
	// Constructor / Destructor
    protected function __construct( $strTag, $hParent, $bAllowAttributes )
    {
		$this->m_strTag = $strTag;
		$this->m_hParent = $hParent;
		$this->m_bAllowAttributes = $bAllowAttributes;

		$this->m_strID = NULL;
		$this->m_iTabIndex = NULL;
		$this->m_strTooltip = NULL;
		$this->m_strContextMenu = NULL;
		$this->m_bEditableContent = false;
		$this->m_bHidden = false;
		$this->m_strLanguage = NULL;

		$this->m_strStyleClasses = NULL;
		$this->m_strStyleInline = NULL;

		$this->m_arrEventCallbacks = array();
		$this->m_arrCustomAttributes = array();
	}
	protected function __destruct()
    {
		// nothing to do
	}

	// Tag & Parent
	public function GetTag() {
		return $this->m_strTag;
	}
	public function GetParent() {
		return $this->m_hParent;
	}
	public function SetParent( $hParent ) {
		$this->m_hParent = $hParent;
	}

	// Global Attributes
	public function AllowsAttributes() {
		return $this->m_bAllowAttributes;
	}

	public function ClearAllAttributes() {
		$this->ClearGlobalAttributes();
	}
	public function ClearGlobalAttributes()
	{
		$this->m_strID = NULL;
		$this->m_iTabIndex = NULL;
		$this->m_strTooltip = NULL;
		$this->m_strContextMenu = NULL;
		$this->m_bEditableContent = false;
		$this->m_bHidden = false;
		$this->m_strLanguage = NULL;

		$this->m_strStyleClasses = NULL;
		$this->m_strStyleInline = NULL;

		$this->m_arrEventCallbacks = array();
		$this->m_arrCustomAttributes = array();
	}

	public function GetID() {
		return $this->m_strID;
	}
	public function SetID( $strID = NULL ) {
        $this->m_strID = $strID;
    }
	public function GetTabIndex() {
		return $this->m_iTabIndex;
	}
	public function SetTabIndex( $iTabIndex = NULL ) {
        $this->m_iTabIndex = $iTabIndex;
    }
	public function GetTooltip() {
		return $this->m_strTooltip;
	}
	public function SetTooltip( $strTooltip = NULL ) {
        $this->m_strTooltip = $strTooltip;
    }
	public function GetContextMenu() {
		return $this->m_strContextMenu;
	}
	public function SetContextMenu( $strContextMenu = NULL ) {
        $this->m_strContextMenu = $strContextMenu;
    }
	public function IsEditableContent() {
		return $this->m_bEditableContent;
	}
	public function SetEditableContent( $bEditableContent = false ) {
        $this->m_bEditableContent = $bEditableContent;
	}
	public function IsHidden() {
		return $this->m_bHidden;
	}
	public function SetHidden( $bHidden = false ) {
        $this->m_bHidden = $bHidden;
	}
	public function GetLanguage() {
		return $this->m_strLanguage;
	}
	public function SetLanguage( $strLanguage = NULL ) {
        $this->m_strLanguage = $strLanguage;
	}

	public function GetStyleClasses() {
		return $this->m_strStyleClasses;
	}
	public function SetStyleClasses( $strClasses ) {
        $this->m_strStyleClasses = $strClasses;
    }
	public function AddStyleClass( $strClass ) {
        $this->m_strStyleClasses .= " $strClassName";
    }
	public function ClearStyleClasses() {
		$this->m_strStyleClasses = NULL;
	}

	public function GetStyleInline() {
		return $this->m_strStyleInline;
	}
	public function SetStyleInline( $strStyle ) {
        $this->m_strStyleInline = $strStyle;
    }
	public function AddStyleInline( $strStyle ) {
        $this->m_strStyleInline .= " $strStyle";
    }
	public function ClearStyleInline() {
		$this->m_strStyleInline = NULL;
	}

	public function GetEventCallback( $strEvent ) {
		if ( isset($this->m_arrEventCallbacks[$strEvent]) )
			return $this->m_arrEventCallbacks[$strEvent];
		return NULL;
	}
	public function SetEventCallback( $strEvent, $strCallback ) {
		$this->m_arrEventCallbacks[$strEvent] = $strCallback;
    }
	public function PrependEventCallback( $strEvent, $strCallback ) {
		$this->m_arrEventCallbacks[$strEvent] = "$strCallback " . $this->m_arrEventCallbacks[$strEvent];
    }
	public function AppendEventCallback( $strEvent, $strCallback ) {
		$this->m_arrEventCallbacks[$strEvent] .= " $strCallback";
    }
	public function ClearEventCallbacks( $strEvent ) {
		$this->m_arrEventCallbacks[$strEvent] = "";
		unset( $this->m_arrEventCallbacks[$strEvent] );
	}
	public function ClearEvents() {
		$this->m_arrEventCallbacks = array();
	}

	public function GetCustomAttribute( $strName ) {
		if ( isset($this->m_arrCustomAttributes[$strName]) )
			return $this->m_arrCustomAttributes[$strName];
		return NULL;
	}
	public function SetCustomAttribute( $strName, $strValue ) {
		$this->m_arrCustomAttributes[$strName] = $strValue;
    }
	public function UnsetCustomAttribute( $strName ) {
		$this->m_arrCustomAttributes[$strName] = "";
		unset( $this->m_arrCustomAttributes[$strName] );
    }
	public function ClearCustomAttributes() {
		$this->m_arrCustomAttributes = array();
	}

	// Helpers
	protected function _GetIndentString( $iIndentDepth ) {
		return "\n" . str_repeat( "\t", $iIndentDepth );
	}

	protected function _ApplyGlobalAttributes( & $strAttributes )
	{
		if ( $this->m_strID != NULL )
			$strAttributes .= " id=\"$this->m_strID\"";
		if ( $this->m_iTabIndex != NULL )
			$strAttributes .= " tabindex=\"$this->m_iTabIndex\"";
		if ( $this->m_strTooltip != NULL )
			$strAttributes .= " title=\"$this->m_strTooltip\"";
		if ( $this->m_strContextMenu != NULL )
			$strAttributes .= " contextmenu=\"$this->m_strContextMenu\"";
		if ( $this->m_bEditableContent )
			$strAttributes .= " contenteditable=\"true\"";
		if ( $this->m_bHidden )
			$strAttributes .= " hidden=\"hidden\"";
		if ( $this->m_strLanguage != NULL )
			$strAttributes .= " lang=\"$this->m_strLanguage\"";

		if ( $this->m_strStyleClasses != NULL )
			$strAttributes .= " class=\"$this->m_strStyleClasses\"";
		if ( $this->m_strStyleInline != NULL )
			$strAttributes .= " style=\"$this->m_strStyleInline\"";

		foreach( $this->m_arrEventCallbacks as $strEvent => $strCallback )
			$strAttributes .= " $strEvent=\"$strCallback\"";
		foreach( $this->m_arrCustomAttributes as $strName => $strValue )
			$strAttributes .= " $strName=\"$strValue\"";
	}

	// Members
	protected $m_strTag;
	protected $m_hParent;
	protected $m_bAllowAttributes;

	protected $m_strID;
	protected $m_iTabIndex;
	protected $m_strTooltip;
	protected $m_strContextMenu;
	protected $m_bEditableContent;
	protected $m_bHidden;
	protected $m_strLanguage;

	protected $m_strStyleClasses;
	protected $m_strStyleInline;

	protected $m_arrEventCallbacks;
	protected $m_arrCustomAttributes;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputTag class
class HTMLInputTag extends HTMLTag
{
	// Constructor / Destructor
    protected function __construct( $strTag, $strInputType, $hParent, $bAllowAttributes )
    {
		parent::__construct( $strTag, $hParent, $bAllowAttributes );

		$this->m_strInputType = $strInputType;

		$this->m_strInputFormID = NULL;
		$this->m_strInputName = NULL;
		$this->m_bInputLocked = false;
		$this->m_bInputRequired = false;
		$this->m_bInputDisabled = false;
		$this->m_bInputAutoFocus = false;
		$this->m_bInputAutoComplete = NULL;
		$this->m_strInputDataList = NULL;
		$this->m_iInputMaxLength = NULL;
		$this->m_iInputCharSize = NULL;
		$this->m_strInputPattern = NULL;
		$this->m_strInputHint = NULL;
		$this->m_iInputMin = NULL;
		$this->m_iInputMax = NULL;
		$this->m_iInputStep = NULL;
	}
	protected function __destruct()
    {
		parent::__destruct();
	}

	// Input Type
	public function GetInputType() {
		return $this->m_strInputType;
	} 

	// Input Attributes
    public function ClearAllAttributes() {
		$this->ClearGlobalAttributes();
		$this->ClearInputAttributes();
	}
	public function ClearInputAttributes()
	{
		$this->m_strInputFormID = NULL;
		$this->m_strInputName = NULL;
		$this->m_bInputLocked = false;
		$this->m_bInputRequired = false;
		$this->m_bInputDisabled = false;
		$this->m_bInputAutoFocus = false;
		$this->m_bInputAutoComplete = NULL;
		$this->m_strInputDataList = NULL;
		$this->m_iInputMaxLength = NULL;
		$this->m_iInputCharSize = NULL;
		$this->m_strInputPattern = NULL;
		$this->m_strInputHint = NULL;
		$this->m_iInputMin = NULL;
		$this->m_iInputMax = NULL;
		$this->m_iInputStep = NULL;
	}

	public function GetInputForm() {
		return $this->m_strInputFormID;
	}
	public function SetInputForm( $strFormID = NULL ) {
		$this->m_strInputFormID = $strFormID;
	}
	public function GetInputName() {
		return $this->m_strInputName;
	}
	public function SetInputName( $strName = NULL ) {
		$this->m_strInputName = $strName;
	}
	public function IsInputLocked() {
		return $this->m_bInputLocked;
	}
	public function SetInputLocked( $bLocked = false ) {
		$this->m_bInputLocked = $bLocked;
	}
	public function IsInputRequired() {
		return $this->m_bInputRequired;
	}
	public function SetInputRequired( $bRequired = false ) {
		$this->m_bInputRequired = $bRequired;
	}
	public function IsInputDisabled() {
		return $this->m_bInputDisabled;
	}
	public function SetInputDisabled( $bDisabled = false ) {
		$this->m_bInputDisabled = $bDisabled;
	}
	public function HasInputAutoFocus() {
		return $this->m_bInputAutoFocus;
	}
	public function SetInputAutoFocus( $bAutoFocus = false ) {
		$this->m_bInputAutoFocus = $bAutoFocus;
	}
	public function HasInputAutoComplete() {
		return $this->m_bInputAutoComplete;
	}
	public function SetInputAutoComplete( $bAutoComplete = NULL ) {
		$this->m_bInputAutoComplete = $bAutoComplete;
	}
	public function GetInputDataList() {
		return $this->m_strInputDataList;
	}
	public function SetInputDataList( $strDataListID = NULL ) {
		$this->m_strInputDataList = $strDataListID;
	}
	public function GetInputMaxLength() {
		return $this->m_iInputMaxLength;
	}
	public function SetInputMaxLength( $iMaxLength = NULL ) {
		$this->m_iInputMaxLength = $iMaxLength;
	}
	public function GetInputCharSize() {
		return $this->m_iInputCharSize;
	}
	public function SetInputCharSize( $iCharSize = NULL ) {
		$this->m_iInputCharSize = $iCharSize;
	}
	public function GetInputPattern() {
		return $this->m_strInputPattern;
	}
	public function SetInputPattern( $strPattern = NULL ) {
		$this->m_strInputPattern = $strPattern;
	}
	public function GetInputHint() {
		return $this->m_strInputHint;
	}
	public function SetInputHint( $strHint = NULL ) {
		$this->m_strInputHint = $strHint;
	}
	public function GetInputMin() {
		return $this->m_iInputMin;
	}
	public function SetInputMin( $iMin = NULL ) {
		$this->m_iInputMin = $iMin;
	}
	public function GetInputMax() {
		return $this->m_iInputMax;
	}
	public function SetInputMax( $iMax = NULL ) {
		$this->m_iInputMax = $iMax;
	}
	public function GetInputStep() {
		return $this->m_iInputStep;
	}
	public function SetInputStep( $iStep = NULL ) {
		$this->m_iInputStep = $iStep;
	}

	// Helpers
	protected function _ApplyInputAttributes( & $strAttributes )
	{
		//static $arrNumerics = array( "number", "range" );
		//static $arrDates = array( "date", "time", "datetime", "datetime-local", "month", "week" );
		static $arrTexts = array( "text", "password", "email", "tel", "url", "search", "textarea" );
		static $arrUIs = array( "button", "checkbox", "radio", "select", "keygen", "reset", "submit", "image" );
		//static $arrMiscs = array( "hidden", "color", "file" );

		$strType = $this->m_strInputType;

		// All
		if ( $this->m_strInputFormID != NULL )
			$strAttributes .= " form=\"$this->m_strInputFormID\"";
		if ( $this->m_strInputName != NULL )
			$strAttributes .= " name=\"$this->m_strInputName\"";

		// All except select & keygen
		if ( $strType != "select" && $strType != "keygen" ) {
			if ( $this->m_bInputLocked )
				$strAttributes .= " readonly=\"readonly\"";
		}

		// Exclude hidden
		if ( $strType == "hidden" )
			return;

		// All remaining
		if ( $this->m_bInputAutoFocus )
			$strAttributes .= " autofocus=\"autofocus\"";
		if ( $this->m_bInputDisabled )
			$strAttributes .= " disabled=\"disabled\"";

		// Exclude UIs
		if ( in_array($strType, $arrUIs) ) {
			if ( $strType == "checkbox" || $strType == "radio" ) {
				if ( $this->m_bInputRequired )
					$strAttributes .= " required=\"required\"";
			}
			return;
		}

		// All remaining except textarea
		if ( $strType != "textarea" ) {
			if ( $this->m_strInputDataList != NULL )
				$strAttributes .= " list=\"$this->m_strInputDataList\"";
		}

		// Exclude Miscs
		if ( $strType == "color" ) {
			if ( $this->m_bInputAutoComplete != NULL ) {
				$strOnOff = $this->m_bInputAutoComplete ? "on" : "off";
				$strAttributes .= " autocomplete=\"$strOnOff\"";
			}
			return;
		}
		if ( $strType == "file" ) {
			if ( $this->m_bInputRequired )
				$strAttributes .= " required=\"required\"";
			return;
		}

		// All remaining except some
		if ( $strType != "range" ) {
			if ( $this->m_bInputRequired )
				$strAttributes .= " required=\"required\"";
		}
		if ( $strType != "number" && $strType != "textarea" ) {
			if ( $this->m_bInputAutoComplete != NULL ) {
				$strOnOff = $this->m_bInputAutoComplete ? "on" : "off";
				$strAttributes .= " autocomplete=\"$strOnOff\"";
			}
		}

		// Exclude texts
		if ( in_array($strType, $arrTexts) ) {
			if ( $this->m_iInputMaxLength != NULL )
				$strAttributes .= " maxlength=\"$this->m_iInputMaxLength\"";
			if ( $this->m_strInputHint != NULL )
				$strAttributes .= " placeholder=\"$this->m_strInputHint\"";
			if ( $strType != "textarea" ) {
				if ( $this->m_iInputCharSize != NULL )
					$strAttributes .= " size=\"$this->m_iInputCharSize\"";
				if ( $this->m_strInputPattern != NULL )
					$strAttributes .= " pattern=\"$this->m_strInputPattern\"";
			}
			return;
		}

		// Numerics or dates remaining
		if ( $this->m_iInputMin != NULL )
			$strAttributes .= " min=\"$this->m_iInputMin\"";
		if ( $this->m_iInputMax != NULL )
			$strAttributes .= " max=\"$this->m_iInputMax\"";
		if ( $this->m_iInputStep != NULL )
			$strAttributes .= " step=\"$this->m_iInputStep\"";
	}

	// Members
	protected $m_strInputType;

	protected $m_strInputFormID;
	protected $m_strInputName;
	protected $m_bInputLocked;
	protected $m_bInputRequired;
	protected $m_bInputDisabled;
	protected $m_bInputAutoFocus;
	protected $m_bInputAutoComplete;
	protected $m_strInputDataList;
	protected $m_iInputMaxLength;
	protected $m_iInputCharSize;
	protected $m_strInputPattern;
	protected $m_strInputHint;
	protected $m_iInputMin;
	protected $m_iInputMax;
	protected $m_iInputStep;
}

?>