<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTML.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 definitions and interface
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

	// DropFunction shortcuts (future DropZone attribute)
define( "HTML_DROPFUNCTION_COPY", "copy" ); // copy dragged data
define( "HTML_DROPFUNCTION_MOVE", "move" ); // move dragged data
define( "HTML_DROPFUNCTION_LINK", "link" ); // create link to original data

	// Document link targets
define( "HTML_TARGET_SELF",   "_self" );   // Same frame (default)
define( "HTML_TARGET_PARENT", "_parent" ); // Parent frame
define( "HTML_TARGET_TOP",    "_top" );    // Top level frame (full body)
define( "HTML_TARGET_BLANK",  "_blank" );  // New window or tab
//const HTML_TARGET_IFRAME            // Named <iframe>

	// Document link relations
define( "HTML_RELATION_ALTERNATE",  "alternate" );
define( "HTML_RELATION_ARCHIVES",   "archives" );
define( "HTML_RELATION_AUTHOR",     "author" );
define( "HTML_RELATION_BOOKMARK",   "bookmark" );
define( "HTML_RELATION_EXTERNAL",   "external" );
define( "HTML_RELATION_FIRST",      "first" );
define( "HTML_RELATION_HELP",       "help" );
define( "HTML_RELATION_ICON",       "icon" );
define( "HTML_RELATION_LAST",       "last" );
define( "HTML_RELATION_LICENSE",    "license" );
define( "HTML_RELATION_NEXT",       "next" );
define( "HTML_RELATION_NOFOLLOW",   "nofollow" );
define( "HTML_RELATION_NOREFERRER", "noreferrer" );
define( "HTML_RELATION_PINGBACK",   "pingback" );
define( "HTML_RELATION_PREFETCH",   "prefetch" );
define( "HTML_RELATION_PREV",       "prev" );
define( "HTML_RELATION_SEARCH",     "search" );
define( "HTML_RELATION_SIDEBAR",    "sidebar" );
define( "HTML_RELATION_STYLESHEET", "stylesheet" );
define( "HTML_RELATION_TAG",        "tag" );
define( "HTML_RELATION_UP",         "up" );

	// Media queries for optimized display
define( "HTML_MEDIAQUERY_DEVICE_ALL",        "all" );
define( "HTML_MEDIAQUERY_DEVICE_AURAL",      "aural" );
define( "HTML_MEDIAQUERY_DEVICE_BRAILLE",    "braille" );
define( "HTML_MEDIAQUERY_DEVICE_HANDHELD",   "handheld" );
define( "HTML_MEDIAQUERY_DEVICE_PROJECTION", "projection" );
define( "HTML_MEDIAQUERY_DEVICE_PRINT",      "print" );
define( "HTML_MEDIAQUERY_DEVICE_SCREEN",     "screen" );
define( "HTML_MEDIAQUERY_DEVICE_TELETYPE",   "tty" );
define( "HTML_MEDIAQUERY_DEVICE_TV",         "tv" );

define( "HTML_MEDIAQUERY_PARAM_WIDTH",          "width" );               //
define( "HTML_MEDIAQUERY_PARAM_HEIGHT",         "height" );              // "min-" and "max-"
define( "HTML_MEDIAQUERY_PARAM_ASPECTRATIO",    "aspect-ratio" );        // prefixes can be used
define( "HTML_MEDIAQUERY_PARAM_DEVWIDTH",       "device-width" );        // on those values ...
define( "HTML_MEDIAQUERY_PARAM_DEVHEIGHT",      "device-height" );       //
define( "HTML_MEDIAQUERY_PARAM_DEVASPECTRATIO", "device-aspect-ratio" ); //
define( "HTML_MEDIAQUERY_PARAM_COLORBITS",      "color" );               //
define( "HTML_MEDIAQUERY_PARAM_COLORS",         "color-index" );         //
define( "HTML_MEDIAQUERY_PARAM_MONOCHROMEBITS", "monochrome" );          //
define( "HTML_MEDIAQUERY_PARAM_ORIENTATION",    "orientation" );         //
define( "HTML_MEDIAQUERY_PARAM_RESOLUTION",     "resolution" ); // dpcm or dpi units
define( "HTML_MEDIAQUERY_PARAM_TVSCANMETHOD",   "scan" ); // 'progressive' or 'interlace'
define( "HTML_MEDIAQUERY_PARAM_GRID",           "grid" ); // 1 => grid, 0 => bitmap

define( "HTML_MEDIAQUERY_PREFIX_MIN", "min" );
define( "HTML_MEDIAQUERY_PREFIX_MAX", "max" );

//media_query_list: <media_query> [, <media_query> ]*
//media_query: [[only | not]? <media_type> [ and <expression> ]*]
//  | <expression> [ and <expression> ]*
//expression: ( <media_feature> [: <value>]? )
//media_type: all | aural | braille | handheld | print |
//  projection | screen | tty | tv | embossed
//media_feature: width | min-width | max-width
//  | height | min-height | max-height
//  | device-width | min-device-width | max-device-width
//  | device-height | min-device-height | max-device-height
//  | aspect-ratio | min-aspect-ratio | max-aspect-ratio
//  | device-aspect-ratio | min-device-aspect-ratio | max-device-aspect-ratio
//  | color | min-color | max-color
//  | color-index | min-color-index | max-color-index
//  | monochrome | min-monochrome | max-monochrome
//  | resolution | min-resolution | max-resolution
//  | scan | grid

	// List's marker types
define( "HTML_LIST_MARKER_DECIMAL", "1" );
define( "HTML_LIST_MARKER_LOWERCASE", "a" );
define( "HTML_LIST_MARKER_UPPERCASE", "A" );
define( "HTML_LIST_MARKER_ROMANLOWER", "i" );
define( "HTML_LIST_MARKER_ROMANUPPER", "I" );

	// Table's head cell's scopes
define( "HTML_TABLE_HEADCELL_SCOPE_COL",      "col" );
define( "HTML_TABLE_HEADCELL_SCOPE_COLGROUP", "colgroup" );
define( "HTML_TABLE_HEADCELL_SCOPE_ROW",      "row" );
define( "HTML_TABLE_HEADCELL_SCOPE_ROWGROUP", "rowgroup" );

	// Menu types
define( "HTML_MENU_LIST",    "list" );    // Simple
define( "HTML_MENU_TOOLBAR", "toolbar" ); // Toolbar
define( "HTML_MENU_CONTEXT", "context" ); // Drop-down

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

	// Text's flags
define( "HTML_TEXT_BOLD",        0x00000001 ); // <b>
define( "HTML_TEXT_ITALIC",      0x00000002 ); // <i>
define( "HTML_TEXT_SMALL",       0x00000004 ); // <small>
define( "HTML_TEXT_SUPER",       0x00000008 ); // <sup>
define( "HTML_TEXT_SUB",         0x00000010 ); // <sub>
define( "HTML_TEXT_PREFORMATED", 0x80000000 ); // <pre>
define( "HTML_TEXT_USAGE_ABBREVIATION", 0x00000001 ); // <abbr>	
define( "HTML_TEXT_USAGE_ADDRESS", 	    0x00000002 ); // <address>
define( "HTML_TEXT_USAGE_CITATION",     0x00000003 ); // <cite>
define( "HTML_TEXT_USAGE_CODE",         0x00000004 ); // <code>
define( "HTML_TEXT_USAGE_DEFINITION",   0x00000005 ); // <dfn>
define( "HTML_TEXT_USAGE_EMPHASIZED",   0x00000006 ); // <em>
define( "HTML_TEXT_USAGE_KEYBOARD",     0x00000007 ); // <kbd>
define( "HTML_TEXT_USAGE_SAMPLE",       0x00000008 ); // <samp>
define( "HTML_TEXT_USAGE_STRONG",       0x00000009 ); // <strong>
define( "HTML_TEXT_USAGE_VARIABLE",     0x0000000a ); // <var>
define( "HTML_TEXT_USAGE_QUOTED",       0x00000010 ); // <q>
define( "HTML_TEXT_USAGE_INSERTED",     0x00000020 ); // <ins>
define( "HTML_TEXT_USAGE_DELETED",      0x00000040 ); // <del>
define( "HTML_TEXT_USAGE_DEPRECATED",   0x00000080 ); // <s>
define( "HTML_TEXT_USAGE_MISSPELLED",   0x80000000 ); // <u>

	// ImageMapArea's shapes
define( "HTML_IMAGEMAP_AREA_SHAPE_DEFAULT", "default" );
define( "HTML_IMAGEMAP_AREA_SHAPE_RECT",    "rect" );
define( "HTML_IMAGEMAP_AREA_SHAPE_CIRCLE",  "circle" );
define( "HTML_IMAGEMAP_AREA_SHAPE_POLY",    "poly" );

	// Audio/Video's preload behaviours
define( "HTML_AUDIOVIDEO_PRELOAD_NONE", "none" );
define( "HTML_AUDIOVIDEO_PRELOAD_META", "metadata" );
define( "HTML_AUDIOVIDEO_PRELOAD_FULL", "auto" );

	// AudioVideoAsset's kinds
define( "HTML_AUDIOVIDEO_ASSET_METADATA",     "metadata" );     // Hidden data
define( "HTML_AUDIOVIDEO_ASSET_CHAPTERS",     "chapters" );     // Media navigation
define( "HTML_AUDIOVIDEO_ASSET_SUBTITLES",    "subtitles" );    // Subtitle data
define( "HTML_AUDIOVIDEO_ASSET_CAPTIONS",     "captions" );     // Deaf users data
define( "HTML_AUDIOVIDEO_ASSET_DESCRIPTIONS", "descriptions" ); // Blind users data

/////////////////////////////////////////////////////////////////////////////////
// The HTMLManager class
class HTMLManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( HTMLManager::$m_hInstance == NULL )
            HTMLManager::$m_hInstance = new HTMLManager();
        return HTMLManager::$m_hInstance;
    }
    protected static $m_hInstance;

    // Constructor / Destructor
    public function __construct()
    {
		$this->m_bIndentDepth = 0;
		
        $this->m_strID = NULL;
		$this->m_iTabIndex = NULL;
		$this->m_strTooltip = NULL;
		$this->m_strContextMenu = NULL;
		$this->m_bEditableContent = false;
		$this->m_bHidden = false;
		$this->m_strLanguage = NULL;
		$this->m_strStyleClass = NULL;
		$this->m_strStyleInline = NULL;
		$this->m_strDragStartFunction = NULL;
		$this->m_strDragOverFunction = NULL;
		$this->m_strDropFunction = NULL;
		$this->m_arrEventCallbacks = NULL;
		$this->m_arrCustomAttributes = NULL;

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

        $this->m_bInHTML = false;
        $this->m_bInHead = false;
        $this->m_bInBody = false;
		$this->m_bInDynamicTag = false;

        $this->m_bInStyle = false;
        $this->m_bInScript = false;

		$this->m_bInHeader = false;
		$this->m_bInFooter = false;
		$this->m_bInSection = false;

		$this->m_bInIFrame = false;
        $this->m_iDivDepth = 0;

		$this->m_bInList = false;
		$this->m_bOrderedList = false;
		$this->m_bInListItem = false;

		$this->m_iTableDepth = 0;
		$this->m_bInTableColGroup = false;
		$this->m_iTableHeaderDepth = 0;
		$this->m_iTableFooterDepth = 0;
		$this->m_iTableBodyDepth = 0;
		$this->m_iTableRowDepth = 0;
		$this->m_iTableHeadCellDepth = 0;
		$this->m_iTableCellDepth = 0;

		$this->m_bInSpan = false;

		$this->m_bInArticle = false;
		$this->m_bInAside = false;
		$this->m_bInTitleGroup = false;
		$this->m_bInTitle = false;
		$this->m_iTitleLevel = 0;
		$this->m_iParagraphDepth = 0;
		$this->m_bInQuote = false;
		$this->m_bInHighLight = false;
		$this->m_iDetailsDepth = 0;
		$this->m_bInDefList = false;
		$this->m_bInDefListItemName = false;
		$this->m_bInDefListItemContent = false;
		$this->m_bInHyperlinkGroup = false;
		$this->m_bInHyperlink = false;

		$this->m_bInDataList = false;
        $this->m_bInComboList = false;
        $this->m_bInComboListGroup = false;
		$this->m_iMenuDepth = 0;
		$this->m_iMenuItemDepth = 0;

        $this->m_bInForm = false;
        $this->m_iFormBlockDepth = 0;

		$this->m_bInMultimedia = false;
		$this->m_bInImageMap = false;
		$this->m_bInAudio = false;
		$this->m_bInVideo = false;
		$this->m_bInObject = false;
    }
	public function __destruct()
    {
		// nothing to do
	}

	// Indentation
	public function GetIndentString() {
		return "\n" . str_repeat( "\t", $this->m_bIndentDepth );
	}

	// Tag attributes support
	public function SetID( $strID ) {
        assert( '$this->m_bInBody' );
        $this->m_strID = $strID;
    }
	public function SetTabIndex( $iTabIndex ) {
        assert( '$this->m_bInBody' );
        $this->m_iTabIndex = $iTabIndex;
    }
	public function SetTooltip( $strTooltip ) {
        assert( '$this->m_bInBody' );
        $this->m_strTooltip = $strTooltip;
    }
	public function SetContextMenu( $strContextMenu ) {
        assert( '$this->m_bInBody' );
        $this->m_strContextMenu = $strContextMenu;
    }
	public function SetEditableContent() {
		assert( '$this->m_bInBody' );
        $this->m_bEditableContent = true;
	}
	public function SetHidden() {
		assert( '$this->m_bInBody' );
        $this->m_bHidden = true;
	}
	public function SetLanguage( $strLanguage ) {
		assert( '$this->m_bInBody' );
        $this->m_strLanguage = $strLanguage;
	}
    public function SetStyleClass( $strClassName ) {
        assert( '$this->m_bInBody' );
        $this->m_strStyleClass = $strClassName;
    }
	public function AddStyleClass( $strClassName ) {
        assert( '$this->m_bInBody' );
        $this->m_strStyleClass .= " $strClassName";
    }
    public function SetStyleInline( $strCSS ) {
        assert( '$this->m_bInBody' );
        $this->m_strStyleInline = $strCSS;
    }
	public function AddStyleInline( $strCSS ) {
        assert( '$this->m_bInBody' );
        $this->m_strStyleInline .= " $strCSS";
    }
    public function SetDragSource( $strOnDragStart = NULL ) {
        assert( '$this->m_bInBody' );
        if ( $strOnDragStart == NULL )
            $this->m_strDragStartFunction = "_DragNDrop_DefaultDragStart";
        else
            $this->m_strDragStartFunction = $strOnDragStart;
    }
    public function SetDropTarget( $strOnDragOver = NULL, $strOnDrop = NULL ) {
        assert( '$this->m_bInBody' );
        if ( $strOnDragOver == NULL )
            $this->m_strDragOverFunction = "_DragNDrop_DefaultDragOver";
        else
            $this->m_strDragOverFunction = $strOnDragOver;
        if ( $strOnDrop == NULL )
            $this->m_strDropFunction = "_DragNDrop_DefaultDrop";
        else
            $this->m_strDropFunction = $strOnDrop;
    }
    public function SetEventCallback( $strEvent, $strCallback ) {
        assert( '$this->m_bInBody' );
		if ( $this->m_arrEventCallbacks == NULL )
			$this->m_arrEventCallbacks = array();
		$this->m_arrEventCallbacks[$strEvent] = $strCallback;
    }
	public function SetCustomAttribute( $strName, $strValue ) {
        assert( '$this->m_bInBody' );
		if ( $this->m_arrCustomAttributes == NULL )
			$this->m_arrCustomAttributes = array();
        $this->m_arrCustomAttributes[$strName] = $strValue;
    }

	public function SetInputForm( $strFormID ) {
		assert( '$this->m_bInBody' );
		$this->m_strInputFormID = $strFormID;
	}
	public function SetInputName( $strName ) {
		assert( '$this->m_bInBody' );
		$this->m_strInputName = $strName;
	}
	public function SetInputLocked( $bLocked ) {
		assert( '$this->m_bInBody' );
		$this->m_bInputLocked = $bLocked;
	}
	public function SetInputRequired( $bRequired ) {
		assert( '$this->m_bInBody' );
		$this->m_bInputRequired = $bRequired;
	}
	public function SetInputDisabled( $bDisabled ) {
		assert( '$this->m_bInBody' );
		$this->m_bInputDisabled = $bDisabled;
	}
	public function SetInputAutoFocus( $bAutoFocus ) {
		assert( '$this->m_bInBody' );
		$this->m_bInputAutoFocus = $bAutoFocus;
	}
	public function SetInputAutoComplete( $bAutoComplete ) {
		assert( '$this->m_bInBody' );
		$this->m_bInputAutoComplete = $bAutoComplete;
	}
	public function SetInputDataList( $strDataListID ) {
		assert( '$this->m_bInBody' );
		$this->m_strInputDataList = $strDataListID;
	}
	public function SetInputMaxLength( $iMaxLength ) {
		assert( '$this->m_bInBody' );
		$this->m_iInputMaxLength = $iMaxLength;
	}
	public function SetInputCharSize( $iCharSize ) {
		assert( '$this->m_bInBody' );
		$this->m_iInputCharSize = $iCharSize;
	}
	public function SetInputPattern( $strPattern ) {
		assert( '$this->m_bInBody' );
		$this->m_strInputPattern = $strPattern;
	}
	public function SetInputHint( $strHint ) {
		assert( '$this->m_bInBody' );
		$this->m_strInputHint = $strHint;
	}
	public function SetInputMin( $iMin ) {
		assert( '$this->m_bInBody' );
		$this->m_iInputMin = $iMin;
	}
	public function SetInputMax( $iMax ) {
		assert( '$this->m_bInBody' );
		$this->m_iInputMax = $iMax;
	}
	public function SetInputStep( $iStep ) {
		assert( '$this->m_bInBody' );
		$this->m_iInputStep = $iStep;
	}

	public function BuildMediaQuery( $strDevice, $strParam, $mValue, $strPrefix = NULL, $strMergeWith = NULL ) {
		if ( $strPrefix != NULL )
			$strQuery = "$strDevice and ($strPrefix-$strParam:$mValue)";
		else
			$strQuery = "$strDevice and ($strParam:$mValue)";
		if ( $strMergeWith != NULL )
			$strQuery = "$strMergeWith, $strQuery";
		return $strQuery;
	}
	
	public function SecureString( $strString, $bSpecialsOnly = true ) {
        if ( $bSpecialsOnly )
            return htmlspecialchars( $strString, ENT_QUOTES | ENT_HTML5 );
        else
            return htmlentities( $strString, ENT_QUOTES | ENT_HTML5 );
    }
    public function UnSecureString( $strString, $bSpecialsOnly = true ) {
        if ( $bSpecialsOnly )
            return htmlspecialchars_decode( $strString, ENT_QUOTES | ENT_HTML5 );
        else
            return html_entity_decode( $strString, ENT_QUOTES | ENT_HTML5 );
    }
	
	public function FormatString( $strString )
	{
		if ( $strString == NULL )
			return NULL;
		
		// Format special text usages
		$strString = str_replace( "[code]", "<div class=\"code_block\"><code class=\"code_content\">", $strString );
		$strString = str_replace( "[/code]", "</code></div>", $strString );
		assert( '$strString != NULL' );
		$strString = str_replace( "[quote]", "<div class=\"quote_block\"><blockquote class=\"quote_content\">", $strString );
		$strString = str_replace( "[/quote]", "</blockquote></div>", $strString );
		assert( '$strString != NULL' );

		/*$strString = preg_replace( "#\[code\](([^\[])*)\[/code\]#",
								   "<div class=\"code_block\">".
									   "<code class=\"code_content\">$1</code>".
								   "</div>",
								   $strString );
		assert( '$strString != NULL' );
		$strString = preg_replace( "#\[quote\](([^\[])*)\[/quote\]#",
								   "<div class=\"quote_block\">".
									   "<blockquote class=\"quote_content\">$1</blockquote>".
								   "</div>",
								   $strString );
		assert( '$strString != NULL' );*/

		// Convert links
		$strString = preg_replace( "#\[link=(([^\]])*)\](([^\[])*)\[/link\]#", "<a href=\"$1\" target=\"_blank\">$3</a>", $strString );
		assert( '$strString != NULL' );
		
		// Convert images
		$strString = preg_replace( "#\[img=(([^\]])*)\]#", "<img src=\"$1\" alt=\"Broken Image\" style=\"max-width:900px;\" />", $strString );
		assert( '$strString != NULL' );

		// Make large and leading spaces non-breaking
		$strString = preg_replace_callback (
			"#((\n|  )( )*)#",
			function( $arrMatches ) {
				$arrMatches[0] = str_replace( " ", "&nbsp", $arrMatches[0] );
				return $arrMatches[0];
			},
			$strString
		);
		assert( '$strString != NULL' );

		// Convert new-lines to line-breaks
		return nl2br( $strString );
	}

	// Comments
	public function Comment( $strComment )
	{
		$strComment = $this->SecureString( $strComment );

		$strIndent = $this->GetIndentString();
        echo "$strIndent<!-- $strComment -->";
	}

	// Document structure
    public function DocumentStart()
    {
        assert( '!($this->m_bInHTML)' );

        $strIndent = $this->GetIndentString();
		echo "$strIndent<!-- HTML 5 page generated by ScarabEngine's PHP Library ... DO NOT EDIT ! -->";
		echo "$strIndent<!DOCTYPE html> ";
        echo "$strIndent<html>";

        $this->m_bInHTML = true;
        ++$this->m_bIndentDepth;
    }
    public function DocumentEnd()
    {
        assert( '$this->m_bInHTML' );

        $this->m_bInHTML = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</html>";
    }
    public function HeadStart()
    {
        assert( '$this->m_bInHTML' );
        assert( '!($this->m_bInHead)' );
        assert( '!($this->m_bInBody)' );

        $strIndent = $this->GetIndentString();
        echo "$strIndent<head>";

        $this->m_bInHead = true;
        ++$this->m_bIndentDepth;
    }
    public function HeadEnd()
    {
        assert( '$this->m_bInHead' );

        $this->m_bInHead = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</head>";
    }
    public function BodyStart()
    {
        assert( '$this->m_bInHTML' );
        assert( '!($this->m_bInHead)' );
        assert( '!($this->m_bInBody)' );

        $strIndent = $this->GetIndentString();
        echo "$strIndent<body>";

        $this->m_bInBody = true;
        ++$this->m_bIndentDepth;
    }
    public function BodyEnd()
    {
        assert( '$this->m_bInBody' );
		assert( '$this->m_iDivDepth == 0' );
		assert( '$this->m_iParagraphDepth == 0' );
		assert( '$this->m_iDetailsDepth == 0' );
		assert( '$this->m_iMenuDepth == 0' );

        $this->m_bInBody = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</body>";
    }

	public function DynamicTagStart( $iIndentDepth = 0 )
	{
		if ( $this->m_bIndentDepth > 0 )
			return;

		$this->m_bInHTML = true;
		$this->m_bInBody = true;
		$this->m_bInDynamicTag = true;
		$this->m_bIndentDepth = 3 + $iIndentDepth;
	}
	public function DynamicTagEnd()
	{
		if ( !($this->m_bInDynamicTag) )
			return;
		
		$this->m_bInHTML = false;
		$this->m_bInBody = false;
		$this->m_bInDynamicTag = false;
		$this->m_bIndentDepth = 0;
	}	

	// Head structure
    public function HeadTitle( $strLabel )
    {
        assert( '$this->m_bInHead' );

		$strTitle = "<title";

		$strTitle = $this->_ApplyTagAttributes( $strTitle );
        $strTitle .= " >";
		
		$strLabel = $this->SecureString( $strLabel );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strTitle$strLabel</title>";
    }

	public function HeadBase( $strDefaultURL, $strDefaultTarget )
	{
		assert( '$this->m_bInHead' );

		$strBase = "<base";
		$strBase .= " href=\"$strDefaultURL\"";
		$strBase .= " target=\"$strDefaultTarget\"";

		$strBase = $this->_ApplyTagAttributes( $strBase );
        $strBase .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strBase";
	}

	public function HeadMetaName( $strName )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta name=\"application-name\" content=\"$strName\" />";
	}
	public function HeadMetaAuthor( $strAuthor )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta name=\"author\" content=\"$strAuthor\" />";
	}
	public function HeadMetaDescription( $strDescription )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta name=\"description\" content=\"$strDescription\" />";
	}
	public function HeadMetaCopyright( $strCopyright )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta name=\"Copyright\" content=\"$strCopyright\" />";
	}
	public function HeadMetaCharset( $strCharset )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta charset=\"$strCharset\" />";
	}
	public function HeadMetaLanguage( $strLanguages )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta http-equiv=\"Content-Language\" content=\"$strLanguages\" />";
	}
	public function HeadMetaRobot( $strRobot )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta name=\"Robot\" content=\"$strRobot\" />";
	}
	public function HeadMetaKeywords( $strKeywords )
	{
		assert( '$this->m_bInHead' );
		$strIndent = $this->GetIndentString();
        echo "$strIndent<meta name=\"keywords\" content=\"$strKeywords\" />";
	}

	public function HeadLink( $strFile, $strType, $strRelation, $strMediaQuery = NULL, $strIconSizes = NULL )
	{
		assert( '$this->m_bInHead' );

		$strLink = "<link";
		$strLink .= " href=\"$strFile\"";
		$strLink .= " type=\"$strType\"";
		$strLink .= " rel=\"$strRelation\"";
		if ( $strMediaQuery != NULL )
			$strLink .= " media=\"$strMediaQuery\"";
		if ( $strRelation == HTML_RELATION_ICON && $strIconSizes != NULL )
			$strLink .= " sizes=\"$strIconSizes\"";

		$strLink = $this->_ApplyTagAttributes( $strLink );
        $strLink .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strLink";
	}

    public function HeadStyleStart( $strMediaQuery = NULL )
    {
        assert( '$this->m_bInHead' );
        assert( '!($this->m_bInStyle)' );

		$strStyle = "<style type=\"text/css\"";
		if ( $strMediaQuery != NULL )
			$strStyle .= " media=\"$strMediaQuery\"";

		$strStyle = $this->_ApplyTagAttributes( $strStyle );
        $strStyle .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strStyle";

        $this->m_bInStyle = true;
        ++$this->m_bIndentDepth;
    }
    public function HeadStyleEnd()
    {
        assert( '$this->m_bInStyle' );

        $this->m_bInStyle = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</style>";
    }
    public function HeadStyleCode( $strCode )
    {
        assert( '$this->m_bInStyle' );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strCode";
    }

	public function HeadScriptNone( $strMessage )
	{
		assert( '$this->m_bInHead' );
		
		$strNoScript = "<noscript";

		$strNoScript = $this->_ApplyTagAttributes( $strNoScript );
        $strNoScript .= " >";
		
		$strMessage = $this->SecureString( $strMessage );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strNoScript$strMessage</noscript>";
	}
	public function HeadScriptExternal( $strURL, $strType, $strCharset = NULL, $bDefer = false, $bAsynchronous = false )
	{
		assert( '$this->m_bInHead' );
		
		$strScript = "<script";
		$strScript .= " src=\"$strURL\"";
		$strScript .= " type=\"$strType\"";
		if ( $strCharset != NULL )
			$strScript .= " charset=\"$strCharset\"";
		if ( $bDefer )
			$strScript .= " defer=\"defer\"";
		if ( $bAsynchronous )
			$strScript .= " async=\"async\"";

		$strScript = $this->_ApplyTagAttributes( $strScript );
        $strScript .= " >";
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strScript</script>";
	}
    public function HeadScriptStart( $strType )
    {
        assert( '$this->m_bInHead' );
        assert( '!($this->m_bInScript)' );

        $strIndent = $this->GetIndentString();
        echo "$strIndent<script type=\"$strType\">";

        $this->m_bInScript = true;
        ++$this->m_bIndentDepth;
    }
    public function HeadScriptEnd()
    {
        assert( '$this->m_bInScript' );

        $this->m_bInScript = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</script>";
    }
    public function HeadScriptCode( $strCode )
    {
        assert( '$this->m_bInScript' );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strCode";
    }

    public function HeadDragNDrop()
    {
        assert( '$this->m_bInHead' );

        $strIndent = $this->GetIndentString();
        echo "$strIndent<script type=\"text/javascript\">";
        echo "$strIndent\tfunction _DragNDrop_AllowDrop(ev) {";
        echo "$strIndent\t    ev.preventDefault();";
        echo "$strIndent\t}";
        echo "$strIndent\tfunction _DragNDrop_DefaultDragStart(ev) {";
        echo "$strIndent\t    ev.dataTransfer.setData(\"Text\",ev.target.id);";
        echo "$strIndent\t}";
        echo "$strIndent\tfunction _DragNDrop_DefaultDragOver(ev) {";
        echo "$strIndent\t    _DragNDrop_AllowDrop(ev);";
        echo "$strIndent\t}";
        echo "$strIndent\tfunction _DragNDrop_DefaultDrop(ev) {";
        echo "$strIndent\t    var data=ev.dataTransfer.getData(\"Text\");";
        echo "$strIndent\t    ev.target.appendChild(document.getElementById(data));";
        echo "$strIndent\t    _DragNDrop_AllowDrop(ev);";
        echo "$strIndent\t}";
        echo "$strIndent</script>";
    }

	// Body structure
	public function HeaderStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInHeader)' );
		assert( '!($this->m_bInFooter)' );
		assert( '!($this->m_bInSection)' );

		$strHeader = "<header";

		$strHeader = $this->_ApplyTagAttributes( $strHeader );
        $strHeader .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strHeader";

		$this->m_bInHeader = true;
        ++$this->m_bIndentDepth;
	}
	public function HeaderEnd()
	{
		assert( '$this->m_bInHeader' );

		$this->m_bInHeader = false;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</header>";
	}
	public function FooterStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInHeader)' );
		assert( '!($this->m_bInFooter)' );
		assert( '!($this->m_bInSection)' );

		$strFooter = "<footer";

		$strFooter = $this->_ApplyTagAttributes( $strFooter );
        $strFooter .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strFooter";

		$this->m_bInFooter = true;
        ++$this->m_bIndentDepth;
	}
	public function FooterEnd()
	{
		assert( '$this->m_bInFooter' );

		$this->m_bInFooter = false;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</footer>";
	}
	public function SectionStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInHeader)' );
		assert( '!($this->m_bInFooter)' );
		assert( '!($this->m_bInSection)' );

		$strSection = "<section";

		$strSection = $this->_ApplyTagAttributes( $strSection );
        $strSection .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strSection";

		$this->m_bInSection = true;
        ++$this->m_bIndentDepth;
	}
	public function SectionEnd()
	{
		assert( '$this->m_bInSection' );

		$this->m_bInSection = false;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</section>";
	}

	public function IFrameStart( $strName, $strSourceURL, $iWidth = NULL, $iHeight = NULL, $bNaked = false, $bSourceCode = false )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInIFrame)' );

		$strIFrame = "<iframe";
		$strIFrame .= " name=\"$strName\"";
		if ( $bSourceCode )
			$strIFrame .= " srcdoc=\"$strSourceURL\"";
		else
			$strIFrame .= " src=\"$strSourceURL\"";
		if ( $iWidth != NULL )
			$strIFrame .= " width=\"$iWidth\"";
		if ( $iHeight != NULL )
			$strIFrame .= " height=\"$iHeight\"";
		if ( $bNaked )
			$strIFrame .= " seamless=\"seamless\"";

		$strIFrame = $this->_ApplyTagAttributes( $strIFrame );
        $strIFrame .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strIFrame";
		
		$this->m_bInIFrame = true;
        ++$this->m_bIndentDepth;
	}
	public function IFrameEnd()
	{
		assert( '$this->m_bInIFrame' );

		$this->m_bInIFrame = false;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</iframe>";
	}

	public function DivStart()
	{
		assert( '$this->m_bInBody' );
		
		$strDiv = "<div";

		$strDiv = $this->_ApplyTagAttributes( $strDiv );
        $strDiv .= " >";
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strDiv";

		++$this->m_iDivDepth;
        ++$this->m_bIndentDepth;
	}
	public function DivEnd()
	{
		assert( '$this->m_iDivDepth > 0' );
		
        --$this->m_iDivDepth;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</div>";
	}

	// Content structure
	public function ListStart( $bOrdered = false, $strMarkerType = NULL, $iStart = NULL, $bReversed = false )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInList)' );

		$strList = "<ul";
		if ( $bOrdered ) {
			$strList = "<ol";
			if ( $strMarkerType != NULL )
				$strList .= " type=\"$strMarkerType\"";
			if ( $iStart != NULL )
				$strList .= " start=\"$iStart\"";
			if ( $bReversed )
				$strList .= " reversed=\"reversed\"";
		}

		$strList = $this->_ApplyTagAttributes( $strList );
        $strList .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strList";

		$this->m_bInList = true;
		$this->m_bOrderedList = $bOrdered;
        ++$this->m_bIndentDepth;
	}
	public function ListEnd()
	{
		assert( '$this->m_bInList' );
		
		$strList = ( $this->m_bOrderedList ? "</ol>" : "</ul>" );

		$this->m_bInList = false;
		$this->m_bOrderedList = NULL;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
		echo "$strIndent$strList";
	}
	public function ListItemStart( $iValue = NULL )
	{
		assert( '$this->m_bInList' );
		assert( '!($this->m_bInListItem)' );
		
		$strListItem = "<li";
		if ( $iValue != NULL )
			$strListItem .= " value=\"$iValue\"";

		$strListItem = $this->_ApplyTagAttributes( $strListItem );
        $strListItem .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strListItem";

		$this->m_bInListItem = true;
        ++$this->m_bIndentDepth;
	}
	public function ListItemEnd()
	{
		assert( '$this->m_bInListItem' );

		$this->m_bInListItem = false;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</li>";
	}

	public function TableStart( $bCellBorders = false )
	{
		assert( '$this->m_bInBody' );
		
		$strTable = "<table";
		if ( $bCellBorders )
			$strTable .= " border=\"1\"";

		$strTable = $this->_ApplyTagAttributes( $strTable );
        $strTable .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTable";

		++$this->m_iTableDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableEnd()
	{
		assert( '$this->m_iTableDepth > 0' );

		--$this->m_iTableDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</table>";
	}
	public function TableCaption( $strCaption )
	{
		assert( '$this->m_iTableDepth > 0' );
		
		$strTableCaption = "<caption";
		
		$strTableCaption = $this->_ApplyTagAttributes( $strTableCaption );
        $strTableCaption .= " >";
		
		$strCaption = $this->SecureString( $strCaption );
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableCaption$strCaption</caption>";
	}
	public function TableColumnGroupStart()
	{
		assert( '$this->m_iTableDepth > 0' );
		assert( '!($this->m_bInTableColGroup)' );
		
		$strTableColumnGroup = "<colgroup";

		$strTableColumnGroup = $this->_ApplyTagAttributes( $strTableColumnGroup );
        $strTableColumnGroup .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableColumnGroup";

		$this->m_bInTableColGroup = true;
        ++$this->m_bIndentDepth;
	}
	public function TableColumnGroupEnd()
	{
		assert( '$this->m_bInTableColGroup' );
		
		$this->m_bInTableColGroup = false;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</colgroup>";
	}
	public function TableColumnGroupItem( $iSpan = NULL )
	{
		assert( '$this->m_bInTableColGroup' );

		$strTableColumnGroupItem = "<col";
		if ( $iSpan != NULL )
			$strTableColumnGroupItem .= " span=\"$iSpan\"";

		$strTableColumnGroupItem = $this->_ApplyTagAttributes( $strTableColumnGroupItem );
        $strTableColumnGroupItem .= " />";
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableColumnGroupItem";
	}
	public function TableHeaderStart()
	{
		assert( '$this->m_iTableDepth > 0' );

		$strTableHeader = "<thead";

		$strTableHeader = $this->_ApplyTagAttributes( $strTableHeader );
        $strTableHeader .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableHeader";

		++$this->m_iTableHeaderDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableHeaderEnd()
	{
		assert( '$this->m_iTableHeaderDepth > 0' );

		--$this->m_iTableHeaderDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</thead>";
	}
	public function TableFooterStart()
	{
		assert( '$this->m_iTableDepth > 0' );
		
		$strTableFooter = "<tfoot";

		$strTableFooter = $this->_ApplyTagAttributes( $strTableFooter );
        $strTableFooter .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableFooter";

		++$this->m_iTableFooterDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableFooterEnd()
	{
		assert( '$this->m_iTableFooterDepth > 0' );

		--$this->m_iTableFooterDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</tfoot>";
	}
	public function TableBodyStart()
	{
		assert( '$this->m_iTableDepth > 0' );
		
		$strTableBody = "<tbody";

		$strTableBody = $this->_ApplyTagAttributes( $strTableBody );
        $strTableBody .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableBody";

		++$this->m_iTableBodyDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableBodyEnd()
	{
		assert( '$this->m_iTableBodyDepth > 0' );

		--$this->m_iTableBodyDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</tbody>";
	}
	public function TableRowStart()
	{
		assert( '$this->m_iTableDepth > 0' );
		
		$strTableRow = "<tr";

		$strTableRow = $this->_ApplyTagAttributes( $strTableRow );
        $strTableRow .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableRow";

		++$this->m_iTableRowDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableRowEnd()
	{
		assert( '$this->m_iTableRowDepth > 0' );
		
		--$this->m_iTableRowDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</tr>";
	}
	public function TableHeadCellStart( $iColumnSpan = NULL, $iRowSpan = NULL, $strScope = NULL, $strHeaderIDs = NULL )
	{
		assert( '$this->m_iTableRowDepth > 0' );

		$strTableHeadCell = "<th";
		if ( $iColumnSpan != NULL )
			$strTableHeadCell .= " colspan=\"$iColumnSpan\"";
		if ( $iRowSpan != NULL )
			$strTableHeadCell .= " rowspan=\"$iRowSpan\"";
		if ( $strScope != NULL )
			$strTableHeadCell .= " scope=\"$strScope\"";
		if ( $strHeaderIDs != NULL )
			$strTableHeadCell .= " headers=\"$strHeaderIDs\"";

		$strTableHeadCell = $this->_ApplyTagAttributes( $strTableHeadCell );
        $strTableHeadCell .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableHeadCell";

		++$this->m_iTableHeadCellDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableHeadCellEnd()
	{
		assert( '$this->m_iTableHeadCellDepth > 0' );
		
		--$this->m_iTableHeadCellDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</th>";
	}
	public function TableCellStart( $iColumnSpan = NULL, $iRowSpan = NULL, $strHeaderIDs = NULL )
	{
		assert( '$this->m_iTableRowDepth > 0' );
		
		$strTableCell = "<td";
		if ( $iColumnSpan != NULL )
			$strTableCell .= " colspan=\"$iColumnSpan\"";
		if ( $iRowSpan != NULL )
			$strTableCell .= " rowspan=\"$iRowSpan\"";
		if ( $strHeaderIDs != NULL )
			$strTableCell .= " headers=\"$strHeaderIDs\"";

		$strTableCell = $this->_ApplyTagAttributes( $strTableCell );
        $strTableCell .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strTableCell";

		++$this->m_iTableCellDepth;
        ++$this->m_bIndentDepth;
	}
	public function TableCellEnd()
	{
		assert( '$this->m_iTableCellDepth > 0' );
		
		--$this->m_iTableCellDepth;
        --$this->m_bIndentDepth;

		$strIndent = $this->GetIndentString();
        echo "$strIndent</td>";
	}

	public function SpanStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInSpan)' );

		$strSpan = "<span";

		$strSpan = $this->_ApplyTagAttributes( $strSpan );
        $strSpan .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strSpan";

		$this->m_bInSpan = true;
        ++$this->m_bIndentDepth;
	}
	public function SpanEnd()
	{
		assert( '$this->m_bInSpan' );

		$this->m_bInSpan = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</span>";
	}

	// Text structure
	public function ArticleStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInArticle)' );

		$strArticle = "<article";

		$strArticle = $this->_ApplyTagAttributes( $strArticle );
        $strArticle .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strArticle";

		$this->m_bInArticle = true;
        ++$this->m_bIndentDepth;
	}
	public function ArticleEnd()
	{
		assert( '$this->m_bInArticle' );

		$this->m_bInArticle = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</article>";
	}
	public function AsideStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInAside)' );
		
		$strAside = "<aside";

		$strAside = $this->_ApplyTagAttributes( $strAside );
        $strAside .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strAside";

		$this->m_bInAside = true;
        ++$this->m_bIndentDepth;
	}
	public function AsideEnd()
	{
		assert( '$this->m_bInAside' );

		$this->m_bInAside = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</aside>";
	}

	public function TitleGroupStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInTitleGroup)' );
		
		$strTitleGroup = "<hgroup";

        $strTitleGroup = $this->_ApplyTagAttributes( $strTitleGroup );
        $strTitleGroup .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strTitleGroup";
		
		$this->m_bInTitleGroup = true;
        ++$this->m_bIndentDepth;
	}
	public function TitleGroupEnd()
	{
		assert( '$this->m_bInTitleGroup' );

		$this->m_bInTitleGroup = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</hgroup>";
	}
	public function TitleStart( $iLevel )
    {
        assert( '$this->m_bInBody' );
		assert( '!($this->m_bInTitle)' );
        assert( '($iLevel >= 1) && ($iLevel <= 6)' );
        
        $strTitle = "<h$iLevel";

        $strTitle = $this->_ApplyTagAttributes( $strTitle );
        $strTitle .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strTitle";
		
		$this->m_bInTitle = true;
		$this->m_iTitleLevel = $iLevel;
        ++$this->m_bIndentDepth;
    }
	public function TitleEnd()
    {
        assert( '$this->m_bInTitle' );

		$iLevel = $this->m_iTitleLevel;

		$this->m_bInTitle = false;
		$this->m_iTitleLevel = 0;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</h$iLevel>";
    }
	public function ParagraphStart()
    {
        assert( '$this->m_bInBody' );

        $strParagraph = "<p";

		$strParagraph = $this->_ApplyTagAttributes( $strParagraph );
        $strParagraph .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strParagraph";
		
		++$this->m_iParagraphDepth;
        ++$this->m_bIndentDepth;
    }
	public function ParagraphEnd()
    {
        assert( '$this->m_iParagraphDepth > 0' );
		
		--$this->m_iParagraphDepth;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</p>";
    }
	public function QuoteStart( $strQuotedURL = NULL )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInQuote)' );

        $strQuote = "<blockquote";
		if ( $strQuotedURL != NULL )
			$strQuote .= " cite=\"$strQuotedURL\"";

		$strQuote = $this->_ApplyTagAttributes( $strQuote );
        $strQuote .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strQuote";

		$this->m_bInQuote = true;
        ++$this->m_bIndentDepth;
	}
	public function QuoteEnd()
	{
		assert( '$this->m_bInQuote' );

		$this->m_bInQuote = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</blockquote>";
	}
	public function HighLightStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInHighLight)' );
		
		$strHighLight = "<mark";

        $strHighLight = $this->_ApplyTagAttributes( $strHighLight );
        $strHighLight .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strHighLight";

		$this->m_bInHighLight = true;
        ++$this->m_bIndentDepth;
	}
	public function HighLightEnd()
	{
		assert( '$this->m_bInHighLight' );

		$this->m_bInHighLight = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</mark>";
	}

	public function DetailsStart( $bVisible = false )
	{
		assert( '$this->m_bInBody' );

		$strDetails = "<details";
		if ( $bVisible )
			$strDetails .= " open=\"open\"";

		$strDetails = $this->_ApplyTagAttributes( $strDetails );
        $strDetails .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strDetails";

		++$this->m_iDetailsDepth;
        ++$this->m_bIndentDepth;
	}
	public function DetailsEnd()
	{
		assert( '$this->m_iDetailsDepth > 0' );

		--$this->m_iDetailsDepth;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</details>";
	}
	public function DetailsCaption( $strCaption )
	{
		assert( '$this->m_iDetailsDepth > 0' );

		$strDetailsCaption = "<summary";

		$strDetailsCaption = $this->_ApplyTagAttributes( $strDetailsCaption );
        $strDetailsCaption .= " >";

		$strCaption = $this->SecureString( $strCaption );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strDetailsCaption$strCaption</summary>";
	}

	public function DefinitionListStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInDefList)' );

        $strDefList = "<dl";

		$strDefList = $this->_ApplyTagAttributes( $strDefList );
        $strDefList .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strDefList";

		$this->m_bInDefList = true;
        ++$this->m_bIndentDepth;
	}
	public function DefinitionListEnd()
	{
		assert( '$this->m_bInDefList' );

		$this->m_bInDefList = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</dl>";
	}
	public function DefinitionListItemNameStart()
	{
		assert( '$this->m_bInDefList' );
		assert( '!($this->m_bInDefListItemName)' );
		assert( '!($this->m_bInDefListItemContent)' );

        $strDefListItemName = "<dt";

		$strDefListItemName = $this->_ApplyTagAttributes( $strDefListItemName );
        $strDefListItemName .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strDefListItemName";

		$this->m_bInDefListItemName = true;
        ++$this->m_bIndentDepth;
	}
	public function DefinitionListItemNameEnd()
	{
		assert( '$this->m_bInDefListItemName' );

		$this->m_bInDefListItemName = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</dt>";
	}
	public function DefinitionListItemContentStart()
	{
		assert( '$this->m_bInDefList' );
		assert( '!($this->m_bInDefListItemName)' );
		assert( '!($this->m_bInDefListItemContent)' );

		$strDefListItemContent = "<dd";

		$strDefListItemContent = $this->_ApplyTagAttributes( $strDefListItemContent );
        $strDefListItemContent .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strDefListItemContent";

		$this->m_bInDefListItemContent = true;
        ++$this->m_bIndentDepth;
	}
	public function DefinitionListItemContentEnd()
	{
		assert( '$this->m_bInDefListItemContent' );
		
		$this->m_bInDefListItemContent = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</dd>";
	}

	public function HyperLinkGroupStart()
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInHyperlinkGroup)' );

		$strHyperLinkGroup = "<nav";

		$strHyperLinkGroup = $this->_ApplyTagAttributes( $strHyperLinkGroup );
        $strHyperLinkGroup .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strHyperLinkGroup";

		$this->m_bInHyperlinkGroup = true;
        ++$this->m_bIndentDepth;
	}
	public function HyperLinkGroupEnd()
	{
		assert( '$this->m_bInHyperlinkGroup' );

		$this->m_bInHyperlinkGroup = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</nav>";
	}
	public function HyperLinkStart( $strURL, $strType = NULL, $strTarget = NULL, $strRelation = NULL, $strMediaQuery = NULL )
    {
        assert( '$this->m_bInBody' );
		assert( '!($this->m_bInHyperlink)' );

        $strHyperLink = "<a";
        $strHyperLink .= " href=\"$strURL\"";
        if ( $strType != NULL )
            $strHyperLink .= " type=\"$strType\"";
        if ( $strTarget != NULL )
            $strHyperLink .= " target=\"$strTarget\"";
		if ( $strRelation != NULL )
			$strHyperLink .= " rel=\"$strRelation\"";
		if ( $strMediaQuery != NULL )
			$strHyperLink .= " media=\"$strMediaQuery\"";

		$strHyperLink = $this->_ApplyTagAttributes( $strHyperLink );
        $strHyperLink .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strHyperLink";

		$this->m_bInHyperlink = true;
        ++$this->m_bIndentDepth;
    }
	public function HyperLinkEnd()
    {
        assert( '$this->m_bInHyperlink' );

		$this->m_bInHyperlink = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</a>";
    }

	// GUI structure
	public function InputLabel( $strTargetID, $strLabel )
    {
        assert( '$this->m_bInBody' );

        $strInputLabel = "<label";
        $strInputLabel .= " for=\"$strTargetID\"";

		$strInputLabel = $this->_ApplyTagAttributes( $strInputLabel );
        $strInputLabel .= " >";

		$strLabel = $this->SecureString( $strLabel );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputLabel$strLabel</label>";
    }

	public function InputDataListStart()
	{
		assert( '$this->m_bInBody' );
        assert( '!($this->m_bInDataList)' );

        $strInputDataList = "<datalist";

        $strInputDataList = $this->_ApplyTagAttributes( $strInputDataList );
        $strInputDataList .= " >";
        
        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputDataList";

        $this->m_bInDataList = true;
        ++$this->m_bIndentDepth;
	}
	public function InputDataListEnd()
	{
		assert( '$this->m_bInDataList' );
        
        $this->m_bInDataList = false;
        --$this->m_bIndentDepth;
        
        $strIndent = $this->GetIndentString();
        echo "$strIndent</datalist>";
	}
	public function InputDataListItem( $strText, $strValue = NULL, $strLabel = NULL, $bSelected = false, $bDisabled = false )
    {
        assert( '$this->m_bInDataList' );

        $strInputDataListItem = "<option";
        if ( $strValue != NULL )
            $strInputDataListItem .= " value=\"$strValue\"";
		if ( $strLabel != NULL )
            $strInputDataListItem .= " label=\"$strLabel\"";
        if ( $bSelected )
            $strInputDataListItem .= " selected=\"selected\"";
        if ( $bDisabled )
            $strInputDataListItem .= " disabled=\"disabled\"";

		$strInputDataListItem = $this->_ApplyTagAttributes( $strInputDataListItem );
        $strInputDataListItem .= " >";

		$strText = $this->SecureString( $strText );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputDataListItem$strText</option>";
    }

	public function InputHidden( $strValue )
    {
        assert( '$this->m_bInBody' );

        $strInputHidden = "<input type=\"hidden\"";
        $strInputHidden .= " value=\"$strValue\"";

		$strInputHidden = $this->_ApplyInputTagAttributes( "hidden", $strInputHidden );
		$strInputHidden = $this->_ApplyTagAttributes( $strInputHidden );
        $strInputHidden .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputHidden";
    }

	public function InputNumber( $iValue = NULL, $bEstimation = false )
	{
		assert( '$this->m_bInBody' );

		$strType = $bEstimation ? "range" : "number";
		$strInputNumber = "<input type=\"$strType\"";
		if ( $iValue != NULL )
            $strInputNumber .= " value=\"$iValue\"";
		
		$strInputNumber = $this->_ApplyInputTagAttributes( $strType, $strInputNumber );
		$strInputNumber = $this->_ApplyTagAttributes( $strInputNumber );
        $strInputNumber .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputNumber";
	}
	public function InputColor( $colValue = NULL )
	{
		assert( '$this->m_bInBody' );

		$strInputColor = "<input type=\"color\"";
		if ( $colValue != NULL )
            $strInputColor .= " value=\"$colValue\"";
		
		$strInputColor = $this->_ApplyInputTagAttributes( "color", $strInputColor );
		$strInputColor = $this->_ApplyTagAttributes( $strInputColor );
        $strInputColor .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputColor";
	}

	public function InputText( $strValue = NULL )
    {
        assert( '$this->m_bInBody' );

        $strInputText = "<input type=\"text\"";
        if ( $strValue != NULL )
            $strInputText .= " value=\"$strValue\"";

		$strInputText = $this->_ApplyInputTagAttributes( "text", $strInputText );
		$strInputText = $this->_ApplyTagAttributes( $strInputText );
        $strInputText .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputText";
    }
	public function InputPassword( /*$strValue = NULL*/ )
    {
        assert( '$this->m_bInBody' );

        $strInputPassword = "<input type=\"password\"";
		//if ( $strValue != NULL )
        //    $strInputPassword .= " value=\"$strValue\"";

		$strInputPassword = $this->_ApplyInputTagAttributes( "password", $strInputPassword );
		$strInputPassword = $this->_ApplyTagAttributes( $strInputPassword );
        $strInputPassword .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputPassword";
    }
	public function InputEMail( $strValue = NULL, $bMultiple = false )
	{
		assert( '$this->m_bInBody' );

		$strInputEMail = "<input type=\"email\"";
		if ( $strValue != NULL )
            $strInputEMail .= " value=\"$strValue\"";
		if ( $bMultiple )
			$strInputEMail .= " multiple=\"multiple\"";
		
		$strInputEMail = $this->_ApplyInputTagAttributes( "email", $strInputEMail );
		$strInputEMail = $this->_ApplyTagAttributes( $strInputEMail );
        $strInputEMail .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputEMail";
	}
	public function InputPhone( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );

		$strInputPhone = "<input type=\"tel\"";
		if ( $strValue != NULL )
            $strInputPhone .= " value=\"$strValue\"";

		$strInputPhone = $this->_ApplyInputTagAttributes( "tel", $strInputPhone );
		$strInputPhone = $this->_ApplyTagAttributes( $strInputPhone );
        $strInputPhone .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputPhone";
	}
	public function InputURL( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );
		
		$strInputURL = "<input type=\"url\"";
		if ( $strValue != NULL )
            $strInputURL .= " value=\"$strValue\"";
		
		$strInputURL = $this->_ApplyInputTagAttributes( "url", $strInputURL );
		$strInputURL = $this->_ApplyTagAttributes( $strInputURL );
        $strInputURL .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputURL";
	}
	public function InputSearch( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );
		
		$strInputSearch = "<input type=\"search\"";
		if ( $strValue != NULL )
            $strInputSearch .= " value=\"$strValue\"";
		
		$strInputSearch = $this->_ApplyInputTagAttributes( "search", $strInputSearch );
		$strInputSearch = $this->_ApplyTagAttributes( $strInputSearch );
        $strInputSearch .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputSearch";
	}

	public function InputDate( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );
		
		$strInputDate = "<input type=\"date\"";
		if ( $strValue != NULL )
            $strInputDate .= " value=\"$strValue\"";
		
		$strInputDate = $this->_ApplyInputTagAttributes( "date", $strInputDate );
		$strInputDate = $this->_ApplyTagAttributes( $strInputDate );
        $strInputDate .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputDate";
	}
	public function InputTime( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );
		
		$strInputTime = "<input type=\"time\"";
		if ( $strValue != NULL )
            $strInputTime .= " value=\"$strValue\"";
		
		$strInputTime = $this->_ApplyInputTagAttributes( "time", $strInputTime );
		$strInputTime = $this->_ApplyTagAttributes( $strInputTime );
        $strInputTime .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputTime";
	}
	public function InputDateTime( $strValue = NULL, $bLocal = false )
	{
		assert( '$this->m_bInBody' );

		$strType = $bLocal ? "datetime-local" : "datetime";
		$strInputDateTime = "<input type=\"$strType\"";
		if ( $strValue != NULL )
            $strInputDateTime .= " value=\"$strValue\"";

		$strInputDateTime = $this->_ApplyInputTagAttributes( $strType, $strInputDateTime );
		$strInputDateTime = $this->_ApplyTagAttributes( $strInputDateTime );
        $strInputDateTime .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputDateTime";
	}
	public function InputMonth( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );
		
		$strInputMonth = "<input type=\"month\"";
		if ( $strValue != NULL )
            $strInputMonth .= " value=\"$strValue\"";

		$strInputMonth = $this->_ApplyInputTagAttributes( "month", $strInputMonth );
		$strInputMonth = $this->_ApplyTagAttributes( $strInputMonth );
        $strInputMonth .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputMonth";
	}
	public function InputWeek( $strValue = NULL )
	{
		assert( '$this->m_bInBody' );
		
		$strInputWeek = "<input type=\"week\"";
		if ( $strValue != NULL )
            $strInputWeek .= " value=\"$strValue\"";

		$strInputWeek = $this->_ApplyInputTagAttributes( "week", $strInputWeek );
		$strInputWeek = $this->_ApplyTagAttributes( $strInputWeek );
        $strInputWeek .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputWeek";
	}

	public function InputTextArea( $iRowCount = 4, $iColumnCount = 24, $strValue = NULL, $bWrapText = NULL )
    {
        assert( '$this->m_bInBody' );

        $strInputTextArea = "<textarea";
        $strInputTextArea .= " rows=\"$iRowCount\"";
        $strInputTextArea .= " cols=\"$iColumnCount\"";
		if ( $bWrapText != NULL ) {
			$strWrap = $bWrapText ? "hard" : "soft";
            $strInputTextArea .= " wrap=\"$strWrap\"";
		}

		$strInputTextArea = $this->_ApplyInputTagAttributes( "textarea", $strInputTextArea );
		$strInputTextArea = $this->_ApplyTagAttributes( $strInputTextArea );
        $strInputTextArea .= " >";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strInputTextArea";
        if ( $strValue != NULL )
            echo $this->SecureString( $strValue );
        echo "</textarea>";
    }
	public function InputFile( $strTypes = NULL, $bMultiple = false )
    {
        assert( '$this->m_bInBody' );
        
        $strInputFile = "<input type=\"file\"";
        if ( $strTypes != NULL )
            $strInputFile .= " accept=\"$strTypes\"";
		if ( $bMultiple )
			$strInputFile .= " multiple=\"multiple\"";

		$strInputFile = $this->_ApplyInputTagAttributes( "file", $strInputFile );
		$strInputFile = $this->_ApplyTagAttributes( $strInputFile );
        $strInputFile .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputFile";
    }

	public function InputButton( $strLabel = "Button" )
    {
        assert( '$this->m_bInBody' );
        
        $strInputButton = "<input type=\"button\"";
        $strInputButton .= " value=\"$strLabel\"";

		$strInputButton = $this->_ApplyInputTagAttributes( "button", $strInputButton );
		$strInputButton = $this->_ApplyTagAttributes( $strInputButton );
        $strInputButton .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputButton";
    }
	public function InputCheckBox( $strValue = NULL, $bChecked = false )
    {
        assert( '$this->m_bInBody' );
        
        $strInputCheckBox = "<input type=\"checkbox\"";
        if ( $strValue != NULL )
            $strInputCheckBox .= " value=\"$strValue\"";
        if ( $bChecked )
            $strInputCheckBox .= " checked=\"checked\"";

		$strInputCheckBox = $this->_ApplyInputTagAttributes( "checkbox", $strInputCheckBox );
		$strInputCheckBox = $this->_ApplyTagAttributes( $strInputCheckBox );
        $strInputCheckBox .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputCheckBox";
    }
	public function InputRadioButton( $strValue = NULL, $bChecked = false )
    {
        assert( '$this->m_bInBody' );
        
        $strInputRadioButton = "<input type=\"radio\"";
        if ( $strValue != NULL )
            $strInputRadioButton .= " value=\"$strValue\"";
        if ( $bChecked )
            $strInputRadioButton .= " checked=\"checked\"";

		$strInputRadioButton = $this->_ApplyInputTagAttributes( "radio", $strInputRadioButton );
		$strInputRadioButton = $this->_ApplyTagAttributes( $strInputRadioButton );
        $strInputRadioButton .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputRadioButton";
    }
	public function InputKeygen( $strKeyType = NULL )
	{
		assert( '$this->m_bInBody' );

		$strInputKeygen = "<keygen";
		if ( $strKeyType != NULL )
			$strInputKeygen .= " keytype=\"$strKeyType\"";

		$strInputKeygen = $this->_ApplyInputTagAttributes( "keygen", $strInputKeygen );
		$strInputKeygen = $this->_ApplyTagAttributes( $strInputKeygen );
        $strInputKeygen .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputKeygen";
	}

	public function InputComboListStart( $iListSize = NULL, $bMultiple = false )
    {
        assert( '$this->m_bInBody' );
        assert( '!($this->m_bInComboList)' );

        $strInputComboList = "<select";
        if ( $iListSize != NULL )
            $strInputComboList .= " size=\"$iListSize\"";
        if ( $bMultiple )
            $strInputComboList .= " multiple=\"multiple\"";
        
        $strInputComboList = $this->_ApplyInputTagAttributes( "combolist", $strInputComboList );
        $strInputComboList = $this->_ApplyTagAttributes( $strInputComboList );
        $strInputComboList .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strInputComboList";

        $this->m_bInComboList = true;
        ++$this->m_bIndentDepth;
    }
    public function InputComboListEnd()
    {
        assert( '$this->m_bInComboList' );

        $this->m_bInComboList = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</select>";
    }
	public function ComboListGroupStart( $strLabel, $bDisabled = false )
    {
        assert( '$this->m_bInComboList' );
        assert( '!($this->m_bInComboListGroup)' );

        $strComboListGroup = "<optgroup";
        $strComboListGroup .= " label=\"$strLabel\"";
        if ( $bDisabled )
            $strComboListGroup .= " disabled=\"disabled\"";

        $strComboListGroup = $this->_ApplyTagAttributes( $strComboListGroup );
        $strComboListGroup .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strComboListGroup";

        $this->m_bInComboListGroup = true;
        ++$this->m_bIndentDepth;
    }
    public function ComboListGroupEnd()
    {
        assert( '$this->m_bInComboListGroup' );
        
        $this->m_bInComboListGroup = false;
        --$this->m_bIndentDepth;
        
        $strIndent = $this->GetIndentString();
        echo "$strIndent</optgroup>";
    }
	public function ComboListItem( $strLabel, $strValue = NULL, $bSelected = false, $bDisabled = false )
    {
        assert( '$this->m_bInComboList' );

        $strComboListItem = "<option";
        $strComboListItem .= " label=\"$strLabel\"";
		if ( $strValue != NULL )
            $strComboListItem .= " value=\"$strValue\"";
        if ( $bSelected )
            $strComboListItem .= " selected=\"selected\"";
        if ( $bDisabled )
            $strComboListItem .= " disabled=\"disabled\"";

		$strComboListItem = $this->_ApplyTagAttributes( $strComboListItem );
        $strComboListItem .= " >";

		$strLabel = $this->SecureString( $strLabel );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strComboListItem$strLabel</option>";
    }

	public function MenuStart( $strType = NULL, $strLabel = NULL )
	{
		assert( '$this->m_bInBody' );
        
        $strMenu = "<menu";
		if ( $strType != NULL )
			$strMenu .= " type=\"$strType\"";
		if ( $strLabel != NULL )
			$strMenu .= " label=\"$strLabel\"";

		$strMenu = $this->_ApplyTagAttributes( $strMenu );
        $strMenu .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strMenu";

		++$this->m_iMenuDepth;
        ++$this->m_bIndentDepth;
	}
	public function MenuEnd()
	{
		assert( '$this->m_iMenuDepth > 0' );
		if ( $this->m_iMenuDepth == 1 )
			assert( '$this->m_iMenuItemDepth == 0' );

		--$this->m_iMenuDepth;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</menu>";
	}
	public function MenuItemStart( $iValue = NULL )
	{
		assert( '$this->m_iMenuDepth > 0' );

		$strMenuItem = "<li";
		if ( $iValue != NULL )
			$strMenuItem .= " value=\"$iValue\"";
			
		$strMenuItem = $this->_ApplyTagAttributes( $strMenuItem );
        $strMenuItem .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strMenuItem";

		++$this->m_iMenuItemDepth;
        ++$this->m_bIndentDepth;
	}
	public function MenuItemEnd()
	{
		assert( '$this->m_iMenuItemDepth > 0' );

		--$this->m_iMenuItemDepth;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</li>";
	}

	public function RatingBar( $iMin, $iMax, $iValue, $iLow = NULL, $iHigh = NULL, $iBest = NULL, $strAltText = "RatingBar" )
	{
		assert( '$this->m_bInBody' );

		$strRatingBar = "<meter";
		$strRatingBar .= " min=\"$iMin\"";
		$strRatingBar .= " max=\"$iMax\"";
		$strRatingBar .= " value=\"$iValue\"";
		if ( $iLow != NULL )
			$strRatingBar .= " low=\"$iLow\"";
		if ( $iHigh != NULL )
			$strRatingBar .= " high=\"$iHigh\"";
		if ( $iBest != NULL )
			$strRatingBar .= " optimum=\"$iBest\"";

		$strRatingBar = $this->_ApplyTagAttributes( $strRatingBar );
        $strRatingBar .= " >";

		$strAltText = $this->SecureString( $strAltText );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strRatingBar$strAltText</meter>";
	}
	public function ProgressBar( $iMax, $iValue = NULL, $strAltText = "ProgressBar" )
	{
		assert( '$this->m_bInBody' );

		$strProgressBar = "<progress";
		$strProgressBar .= " max=\"$iMax\"";
		if ( $iValue != NULL )
			$strProgressBar .= " value=\"$iValue\"";

		$strProgressBar = $this->_ApplyTagAttributes( $strProgressBar );
        $strProgressBar .= " >";

		$strAltText = $this->SecureString( $strAltText );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strProgressBar$strAltText</progress>";
	}

	public function Command( $strType, $strLabel, $strText, $strIconURL = NULL, $strRadioGroup = NULL, $bChecked = false, $bDisabled = false )
	{
		assert( '$this->m_bInBody' );

		$strCommand = "<command";
		$strCommand .= " type=\"$strType\"";
		$strCommand .= " label=\"$strLabel\"";
		if ( $strIconURL != NULL )
            $strCommand .= " icon=\"$strIconURL\"";
		if ( ($strType == HTML_COMMAND_TYPE_RADIO) && ($strRadioGroup != NULL) )
            $strCommand .= " radiogroup=\"$strRadioGroup\"";
		if ( ($strType != HTML_COMMAND_TYPE_COMMAND) && $bChecked )
            $strCommand .= " checked=\"checked\"";
		if ( $bDisabled )
            $strCommand .= " disabled=\"disabled\"";

		$strCommand = $this->_ApplyTagAttributes( $strCommand );
        $strCommand .= " >";

		$strText = $this->SecureString( $strText );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strCommand$strText</command>";
	}

	// Forms structure
	public function FormStart( $strName, $strAction, $strMethod = NULL, $strTarget = NULL,
							   $strEncType = NULL, $strCharsets = NULL, $bAutoComplete = NULL, $bNoValidation = false )
    {
        assert( '$this->m_bInBody' );
        assert( '!($this->m_bInForm)' );

        $strForm = "<form";
        $strForm .= " name=\"$strName\"";
		$strForm .= " action=\"$strAction\"";
        if ( $strMethod != NULL )
            $strForm .= " method=\"$strMethod\"";
		if ( $strTarget != NULL )
            $strForm .= " target=\"$strTarget\"";
        if ( $strEncType != NULL )
            $strForm .= " enctype=\"$strEncType\"";
		if ( $strCharsets != NULL )
            $strForm .= " accept-charset=\"$strCharsets\"";
		if ( $bAutoComplete != NULL ) {
			$strOnOff = $bAutoComplete ? "on" : "off";
            $strForm .= " autocomplete=\"$strOnOff\"";
		}
		if ( $bNoValidation )
			$strForm .= " novalidate=\"novalidate\"";

		$strForm = $this->_ApplyTagAttributes( $strForm );
        $strForm .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strForm";

        $this->m_bInForm = true;
        ++$this->m_bIndentDepth;
    }
    public function FormEnd()
    {
        assert( '$this->m_bInForm' );
        assert( '$this->m_iFormBlockDepth == 0' );

        $this->m_bInForm = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</form>";
    }

	public function FormBlockStart( $strName, $bDisabled = false )
    {
        assert( '$this->m_bInForm' );
        
        $strFormBlock = "<fieldset";
		$strFormBlock .= " name=\"$strName\"";
		if ( $bDisabled )
            $strFormBlock .= " disabled=\"disabled\"";

		$strFormBlock = $this->_ApplyTagAttributes( $strFormBlock );
        $strFormBlock .= " >";
        
        $strIndent = $this->GetIndentString();
        echo "$strIndent$strFormBlock";

        ++$this->m_iFormBlockDepth;
        ++$this->m_bIndentDepth;
    }
    public function FormBlockEnd()
    {
        assert( '$this->m_iFormBlockDepth > 0' );

        --$this->m_iFormBlockDepth;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</fieldset>";
    }
	public function FormBlockLegend( $strLegend )
	{
		assert( '$this->m_iFormBlockDepth > 0' );

		$strFormBlockLegend = "<legend";
		
		$strFormBlockLegend = $this->_ApplyTagAttributes( $strFormBlock );
        $strFormBlockLegend .= " >";

		$strLegend = $this->SecureString( $strLegend );
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strFormBlockLegend$strLegend</legend>";
	}

	public function FormOutput( $strName, $strInputIDs, $strDefault = "" )
	{
		assert( '$this->m_bInForm' );

		$strFormOutput = "<output";
		$strFormOutput .= " name=\"$strName\"";
		$strFormOutput .= " for=\"$strInputIDs\"";

		$strFormOutput = $this->_ApplyTagAttributes( $strFormOutput );
        $strFormOutput .= " >";

		$strDefault = $this->SecureString( $strDefault );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strFormOutput$strDefault</output>";
	}

	public function FormResetButton( $strLabel = "Reset" )
    {
        assert( '$this->m_bInForm' );

        $strFormResetButton = "<input type=\"reset\"";
        $strFormResetButton .= " value=\"$strLabel\"";

		$strFormResetButton = $this->_ApplyInputTagAttributes( "reset", $strFormResetButton );
		$strFormResetButton = $this->_ApplyTagAttributes( $strFormResetButton );
        $strFormResetButton .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strFormResetButton";
    }
	public function FormSubmitButton( $strLabel = "Submit" )
    {
        assert( '$this->m_bInForm' );
        
        $strFormSubmitButton = "<input type=\"submit\"";
        $strFormSubmitButton .= " value=\"$strLabel\"";

		$strFormSubmitButton = $this->_ApplyInputTagAttributes( "submit", $strFormSubmitButton );
		$strFormSubmitButton = $this->_ApplyTagAttributes( $strFormSubmitButton );
        $strFormSubmitButton .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strFormSubmitButton";
    }
	public function FormSubmitImage( $strSourceURL, $iWidth, $iHeigh, $strAltText = "Submit", $strValue = NULL )
    {
        assert( '$this->m_bInForm' );

        $strFormSubmitImage = "<input type=\"image\"";
        $strFormSubmitImage .= " src=\"$strSourceURL\"";
        $strFormSubmitImage .= " width=\"$iWidth\"";
        $strFormSubmitImage .= " height=\"$iHeigh\"";
        $strFormSubmitImage .= " alt=\"$strAltText\"";
        if ( $strValue != NULL )
            $strFormSubmitImage .= " value=\"$strValue\"";

		$strFormSubmitImage = $this->_ApplyInputTagAttributes( "image", $strFormSubmitImage );
		$strFormSubmitImage = $this->_ApplyTagAttributes( $strFormSubmitImage );
        $strFormSubmitImage .= " />";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strFormSubmitImage";
    }

	// Text content
	public function TextRaw( $strText )
	{
		assert( '$this->m_bInBody' );

		$strIndent = $this->GetIndentString();
		
		echo "$strIndent$strText";
	}
	public function Text( $strText, $iTextFlag = 0, $iUsageFlag = 0, $iAutoBreak = NULL )
    {
        assert( '$this->m_bInBody' );
		
		$strOpen = "";
        $strClose = "";
		$strIndent = $this->GetIndentString();

        // Usage flag
        switch( ($iUsageFlag & 0x0000000f) ) {
            case HTML_TEXT_USAGE_ABBREVIATION: $strOpen = "<abbr>"; $strClose = "</abbr>"; break;
            case HTML_TEXT_USAGE_ADDRESS:      $strOpen = "<address>"; $strClose = "</address>"; break;
            case HTML_TEXT_USAGE_CITATION:     $strOpen = "<cite>"; $strClose = "</cite>"; break;
            case HTML_TEXT_USAGE_CODE:         $strOpen = "<code>"; $strClose = "</code>"; break;
            case HTML_TEXT_USAGE_DEFINITION:   $strOpen = "<dfn>"; $strClose = "</dfn>"; break;
            case HTML_TEXT_USAGE_EMPHASIZED:   $strOpen = "<em>"; $strClose = "</em>"; break;
            case HTML_TEXT_USAGE_KEYBOARD:     $strOpen = "<kbd>"; $strClose = "</kbd>"; break;
            case HTML_TEXT_USAGE_SAMPLE:       $strOpen = "<samp>"; $strClose = "</samp>"; break;
            case HTML_TEXT_USAGE_STRONG:       $strOpen = "<strong>"; $strClose = "</strong>"; break;
            case HTML_TEXT_USAGE_VARIABLE:     $strOpen = "<var>"; $strClose = "</var>"; break;
            default: break;
        }
		if ( $iUsageFlag & HTML_TEXT_USAGE_QUOTED ) { $strOpen .= "<q>"; $strClose = "</q>$strClose"; }
		if ( $iUsageFlag & HTML_TEXT_USAGE_INSERTED )        { $strOpen .= "<ins>"; $strClose = "</ins>$strClose"; }
		else if ( $iUsageFlag & HTML_TEXT_USAGE_DELETED )    { $strOpen .= "<del>"; $strClose = "</del>$strClose"; }
		else if ( $iUsageFlag & HTML_TEXT_USAGE_DEPRECATED ) { $strOpen .= "<s>"; $strClose = "</s>$strClose"; }

		if ( $iUsageFlag & HTML_TEXT_USAGE_MISSPELLED ) { $strOpen .= "<u>"; $strClose = "</u>$strClose"; }

        // Text flag
        if ( $iTextFlag & HTML_TEXT_BOLD )   { $strOpen .= "<b>"; $strClose = "</b>$strClose"; }
        if ( $iTextFlag & HTML_TEXT_ITALIC ) { $strOpen .= "<i>"; $strClose = "</i>$strClose"; }
        if ( $iTextFlag & HTML_TEXT_SMALL )  { $strOpen .= "<small>"; $strClose = "</small>$strClose"; }
        if ( $iTextFlag & HTML_TEXT_SUPER )    { $strOpen .= "<sup>"; $strClose = "</sup>$strClose"; }
        else if ( $iTextFlag & HTML_TEXT_SUB ) { $strOpen .= "<sub>"; $strClose = "</sub>$strClose"; }

		if ( $iTextFlag & HTML_TEXT_PREFORMATED ) { $strOpen .= "<pre>"; $strClose = "</pre>$strClose"; }

		// Text indentation
		$strTextIndent = ( $strOpen != "" ) ? "$strIndent\t" : "$strIndent";

		// Auto-break
		if ( $iAutoBreak != NULL && $iAutoBreak > 0 )
			$strText = wordwrap( $strText, $iAutoBreak, "$strTextIndent<br/>" );

		// Text Formatting
		$strText = $this->SecureString( $strText );
		$strText = $this->FormatString( $strText );

		// Print
		if ( $strOpen != "" )
			echo "$strIndent$strOpen";
		echo "$strTextIndent$strText";
		if ( $strClose != "" )
			echo "$strIndent$strClose";
    }
	public function DateTime( $strDateTime, $bIsPublicationDate = false, $strAltText = "DateTime" )
	{
		assert( '$this->m_bInBody' );

		$strDateTime = "<time";
		$strDateTime .= " datetime=\"$strDateTime\"";
		if ( $bIsPublicationDate )
			$strDateTime .= " pubdate=\"pubdate\"";

		$strDateTime = $this->_ApplyTagAttributes( $strDateTime );
        $strDateTime .= " >";

		$strAltText = $this->SecureString( $strAltText );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strDateTime$strAltText</time>";
	}

	public function LineBreak() {
        assert( '$this->m_bInBody' );
        $strIndent = $this->GetIndentString();
        echo "$strIndent<br/>";
    }
	public function WordBreak() {
        assert( '$this->m_bInBody' );
        $strIndent = $this->GetIndentString();
        echo "$strIndent<wbr/>";
    }
    public function HorizontalLine() {
        assert( '$this->m_bInBody' );
        $strIndent = $this->GetIndentString();
        echo "$strIndent<hr/>";
    }

	// Multimedia content
	public function MultimediaStart()
    {
        assert( '$this->m_bInBody' );
		assert( '!($this->m_bInMultimedia)' );
        
        $strMultimedia = "<figure";

		$strMultimedia = $this->_ApplyTagAttributes( $strMultimedia );
        $strMultimedia .= " >";

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strMultimedia";

		$this->m_bInMultimedia = true;
        ++$this->m_bIndentDepth;
    }
	public function MultimediaEnd()
    {
        assert( '$this->m_bInMultimedia' );
		
		$this->m_bInMultimedia = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</figure>";
    }
	public function MultimediaCaption( $strCaption )
    {
        assert( '$this->m_bInMultimedia' );

        $strMultimediaCaption = "<figcaption";

		$strMultimediaCaption = $this->_ApplyTagAttributes( $strMultimediaCaption );
        $strMultimediaCaption .= " >";

		$strCaption = $this->SecureString( $strCaption );

        $strIndent = $this->GetIndentString();
        echo "$strIndent$strMultimediaCaption$strCaption</figcaption>";
    }

	// Image content
	public function Image( $strImageSource, $strAltText = "Image", $iWidth = NULL, $iHeight = NULL,
						   $bIsServerMap = false, $strClientMap = NULL )
	{
		assert( '$this->m_bInBody' );

		$strImage = "<img";
		$strImage .= " src=\"$strImageSource\"";
        $strImage .= " alt=\"$strAltText\"";
		if ( $iWidth != NULL )
			$strImage .= " width=\"$iWidth\"";
		if ( $iHeight != NULL )
			$strImage .= " height=\"$iHeight\"";
		if ( $bIsServerMap )
			$strImage .= " ismap=\"ismap\"";
		if ( $strClientMap != NULL )
			$strImage .= " usemap=\"#$strClientMap\"";

		$strImage = $this->_ApplyTagAttributes( $strImage );
        $strImage .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strImage";
	}
	public function ImageMapStart( $strName )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInImageMap)' );

		$strImageMap = "<map";
		$strImageMap .= " name=\"$strName\"";
		
		$strImageMap = $this->_ApplyTagAttributes( $strImageMap );
        $strImageMap .= " >";
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strImageMap";

        $this->m_bInImageMap = true;
        ++$this->m_bIndentDepth;
	}
	public function ImageMapEnd()
	{
		assert( '$this->m_bInImageMap' );
		
		$this->m_bInImageMap = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</map>";
	}
	public function ImageMapArea( $strShape, $strCoords, $strURL, $strAltText = "Area", $strType = NULL, $strTarget = NULL,
							      $strRelation = NULL, $strMediaQuery = NULL )
	{
		assert( '$this->m_bInImageMap' );

		$strImageMapArea = "<area";
		$strImageMapArea .= " shape=\"$strShape\"";
		$strImageMapArea .= " coords=\"$strCoords\"";
		$strImageMapArea .= " href=\"$strURL\"";
		$strImageMapArea .= " alt=\"$strAltText\"";
		if ( $strType != NULL )
			$strImageMapArea .= " type=\"$strType\"";
		if ( $strTarget != NULL )
			$strImageMapArea .= " target=\"$strTarget\"";
		if ( $strRelation != NULL )
			$strImageMapArea .= " rel=\"$strRelation\"";
		if ( $strMediaQuery != NULL )
			$strImageMapArea .= " media=\"$strMediaQuery\"";

		$strImageMapArea = $this->_ApplyTagAttributes( $strImageMapArea );
        $strImageMapArea .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strImageMapArea";
	}

	// Audio/Video content
	public function AudioStart( $strSourceURL, $bShowControls = false, $bAutoPlay = false, $bLoop = false,
	                            $strPreload = NULL, $strAltText = "Audio" )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInAudio)' );

		$strAudio = "<audio";
		$strAudio .= " src=\"$strSourceURL\"";
		if ( $bShowControls )
			$strAudio .= " controls=\"controls\"";
		if ( $bAutoPlay )
			$strAudio .= " autoplay=\"autoplay\"";
		if ( $bLoop )
			$strAudio .= " loop=\"loop\"";
		if ( $strPreload != NULL )
			$strAudio .= " preload=\"$strPreload\"";

		$strAudio = $this->_ApplyTagAttributes( $strAudio );
        $strAudio .= " >";

		$strAltText = $this->SecureString( $strAltText );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strAudio";
		echo "$strIndent\t$strAltText";

        $this->m_bInAudio = true;
        ++$this->m_bIndentDepth;
	}
	public function AudioEnd()
	{
		assert( '$this->m_bInAudio' );

		$this->m_bInAudio = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</audio>";
	}
	public function VideoStart( $strSourceURL, $strSplashURL = NULL, $iWidth = NULL, $iHeight = NULL,
								$bShowControls = false, $bAutoPlay = false, $bLoop = false, $bMute = false,
							    $strPreload = NULL, $strAltText = "Video" )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInVideo)' );

		$strVideo = "<video";
		$strVideo .= " src=\"$strSourceURL\"";
		if ( $strSplashURL != NULL )
			$strVideo .= " poster=\"$strSplashURL\"";
		if ( $iWidth != NULL )
			$strVideo .= " width=\"$iWidth\"";
		if ( $iHeight != NULL )
			$strVideo .= " height=\"$iHeight\"";
		if ( $bShowControls )
			$strVideo .= " controls=\"controls\"";
		if ( $bAutoPlay )
			$strVideo .= " autoplay=\"autoplay\"";
		if ( $bLoop )
			$strVideo .= " loop=\"loop\"";
		if ( $bMute )
			$strVideo .= " muted=\"muted\"";
		if ( $strPreload != NULL )
			$strVideo .= " preload=\"$strPreload\"";

		$strVideo = $this->_ApplyTagAttributes( $strVideo );
        $strVideo .= " >";

		$strAltText = $this->SecureString( $strAltText );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strVideo";
		echo "$strIndent\t$strAltText";

        $this->m_bInVideo = true;
        ++$this->m_bIndentDepth;
	}
	public function VideoEnd()
	{
		assert( '$this->m_bInVideo' );

		$this->m_bInVideo = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</video>";
	}
	public function AudioVideoSource( $strSourceURL, $strType = NULL, $strMediaQuery = NULL )
	{
		assert( '$this->m_bInAudio || $this->m_bInVideo' );

		$strAudVidSource = "<source";
		$strAudVidSource .= " src=\"$strSourceURL\"";
		if ( $strType != NULL )
			$strAudVidSource .= " type=\"$strType\"";
		if ( $strMediaQuery != NULL )
			$strAudVidSource .= " media=\"$strMediaQuery\"";

		$strAudVidSource = $this->_ApplyTagAttributes( $strAudVidSource );
        $strAudVidSource .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strAudVidSource";
	}
	public function AudioVideoAsset( $strSourceURL, $strKind, $strLabel = NULL, $bDefault = false, $strLanguage = NULL )
	{
		assert( '$this->m_bInAudio || $this->m_bInVideo' );
		if ( $strKind == HTML_AUDIOVIDEO_ASSET_SUBTITLES )
			assert( '$strLanguage != NULL' );

		$strAudVidAsset = "<track";
		$strAudVidAsset .= " src=\"$strSourceURL\"";
		$strAudVidAsset .= " kind=\"$strKind\"";
		if ( $strLabel != NULL )
			$strAudVidAsset .= " label=\"$strLabel\"";
		if ( $bDefault )
			$strAudVidAsset .= " default=\"default\"";
		if ( $strLanguage != NULL )
			$strAudVidAsset .= " srclang=\"$strLanguage\"";

		$strAudVidAsset = $this->_ApplyTagAttributes( $strAudVidAsset );
        $strAudVidAsset .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strAudVidAsset";
	}

	// Custom content
	public function Embedded( $strSourceURL, $iWidth = NULL, $iHeight = NULL, $strType = NULL )
	{
		assert( '$this->m_bInBody' );

		$strEmbedded = "<embed";
		$strEmbedded .= " src=\"$strSourceURL\"";
		if ( $iWidth != NULL )
			$strEmbedded .= " width=\"$iWidth\"";
		if ( $iHeight != NULL )
			$strEmbedded .= " height=\"$iHeight\"";
		if ( $strType != NULL )
			$strEmbedded .= " type=\"$strType\"";

		$strEmbedded = $this->_ApplyTagAttributes( $strEmbedded );
        $strEmbedded .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strEmbedded";
	}
	public function ObjectStart( $strName, $strSourceURL, $iWidth, $iHeight, $strType = NULL,
								 $strClientMap = NULL, $strAltText = "Object" )
	{
		assert( '$this->m_bInBody' );
		assert( '!($this->m_bInObject)' );

		$strObject = "<object";
		$strObject .= " name=\"$strName\"";
		$strObject .= " data=\"$strSourceURL\"";
		$strObject .= " width=\"$iWidth\"";
		$strObject .= " height=\"$iHeight\"";
		if ( $strType != NULL )
			$strObject .= " type=\"$strType\"";
		if ( $strClientMap != NULL )
			$strObject .= " usemap=\"#$strClientMap\"";
		
		$strObject = $this->_ApplyTagAttributes( $strObject );
        $strObject .= " >";

		$strAltText = $this->SecureString( $strAltText );
		
		$strIndent = $this->GetIndentString();
        echo "$strIndent$strObject";
		echo "$strIndent\t$strAltText";

        $this->m_bInObject = true;
        ++$this->m_bIndentDepth;
	}
	public function ObjectEnd()
	{
		assert( '$this->m_bInObject' );

		$this->m_bInObject = false;
        --$this->m_bIndentDepth;

        $strIndent = $this->GetIndentString();
        echo "$strIndent</object>";
	}
	public function ObjectParameter( $strName, $mValue )
	{
		assert( '$this->m_bInObject' );

		$strObjectParam = "<param";
		$strObjectParam .= " name=\"$strName\"";
		$strObjectParam .= " value=\"$mValue\"";

		$strObjectParam = $this->_ApplyTagAttributes( $strObjectParam );
        $strObjectParam .= " />";

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strObjectParam";
	}

	// Real-time graphics
	public function Graphics2D( $iWidth = NULL, $iHeight = NULL, $strAltText = "Graphics2D" )
	{
		assert( '$this->m_bInBody' );
		
		$strGraphics2D = "<canvas";
		if ( $iWidth != NULL )
			$strGraphics2D .= " width=\"$iWidth\"";
		if ( $iHeight != NULL )
			$strGraphics2D .= " height=\"$iHeight\"";

		$strGraphics2D = $this->_ApplyTagAttributes( $strGraphics2D );
        $strGraphics2D .= " >";

		$strAltText = $this->SecureString( $strAltText );

		$strIndent = $this->GetIndentString();
        echo "$strIndent$strGraphics2D$strAltText</canvas>";
	}

	// Helpers
	private function _ApplyTagAttributes( $strTag )
	{
		if ( $this->m_strID != NULL ) {
            $strTag .= " id=\"$this->m_strID\"";
			$this->m_strID = NULL;
        }
		if ( $this->m_iTabIndex != NULL ) {
			$strTag .= " tabindex=\"$this->m_iTabIndex\"";
			$this->m_iTabIndex = NULL;
        }
		if ( $this->m_strTooltip != NULL ) {
			$strTag .= " title=\"$this->m_strTooltip\"";
			$this->m_strTooltip = NULL;
        }
		if ( $this->m_strContextMenu != NULL ) {
			$strTag .= " contextmenu=\"$this->m_strContextMenu\"";
			$this->m_strContextMenu = NULL;
        }
		if ( $this->m_bEditableContent ) {
			$strTag .= " contenteditable=\"true\"";
			$this->m_bEditableContent = false;
		}
		if ( $this->m_bHidden ) {
			$strTag .= " hidden=\"hidden\"";
			$this->m_bHidden = false;
		}
		if ( $this->m_strLanguage != NULL ) {
			$strTag .= " lang=\"$this->m_strLanguage\"";
			$this->m_strLanguage = NULL;
		}
        if ( $this->m_strStyleClass != NULL ) {
            $strTag .= " class=\"$this->m_strStyleClass\"";
			$this->m_strStyleClass = NULL;
		}
        if ( $this->m_strStyleInline != NULL ) {
            $strTag .= " style=\"$this->m_strStyleInline\"";
			$this->m_strStyleInline = NULL;
		}
        if ( $this->m_strDragStartFunction != NULL ) {
            $strTag .= " draggable=\"true\" ondragstart=\"$this->m_strDragStartFunction(event)\"";
            $this->m_strDragStartFunction = NULL;
        }
		if ( $this->m_strDragOverFunction != NULL ) {
			$strTag .= " ondragover=\"$this->m_strDragOverFunction(event)\"";
            $this->m_strDragOverFunction = NULL;
		}
		if ( $this->m_strDropFunction != NULL ) {
			if ( $this->m_strDropFunction == HTML_DROPFUNCTION_COPY || $this->m_strDropFunction == HTML_DROPFUNCTION_MOVE
			     || $this->m_strDropFunction == HTML_DROPFUNCTION_LINK )
				$strTag .= " dropzone=\"$this->m_strDropFunction\"";
			else
				$strTag .= " ondrop=\"$this->m_strDropFunction(event)\"";
            $this->m_strDropFunction = NULL;
		}
		if ( $this->m_arrEventCallbacks != NULL ) {
			foreach( $this->m_arrEventCallbacks as $strEvent => $strCallback )
				$strTag .= " $strEvent=\"$strCallback\"";
			unset( $this->m_arrEventCallbacks );
			$this->m_arrEventCallbacks = NULL;
		}
		if ( $this->m_arrCustomAttributes != NULL ) {
			foreach( $this->m_arrCustomAttributes as $strName => $strValue )
				$strTag .= " $strName=\"$strValue\"";
			unset( $this->m_arrCustomAttributes );
			$this->m_arrCustomAttributes = NULL;
		}

		return $strTag;
	}
	private function _ApplyInputTagAttributes( $strType, $strTag )
	{
		//static $arrNumerics = array( "number", "range" );
		//static $arrDates = array( "date", "time", "datetime", "datetime-local", "month", "week" );
		static $arrTexts = array( "text", "password", "email", "tel", "url", "search", "textarea" );
		static $arrUIs = array( "button", "checkbox", "radio", "combolist", "keygen", "reset", "submit", "image" );
		//static $arrMiscs = array( "hidden", "color", "file" );

		// All
		if ( $this->m_strInputFormID != NULL ) {
			$strTag .= " form=\"$this->m_strInputFormID\"";
			$this->m_strInputFormID = NULL;
		}
		if ( $this->m_strInputName != NULL ) {
			$strTag .= " name=\"$this->m_strInputName\"";
			$this->m_strInputName = NULL;
		}
		if ( $strType != "combolist" && $strType != "keygen" ) {
			if ( $this->m_bInputLocked ) {
				$strTag .= " readonly=\"readonly\"";
				$this->m_bInputLocked = false;
			}
		}

		// Exclude hidden
		if ( $strType == "hidden" )
			return $strTag;

		// All remaining
		if ( $this->m_bInputAutoFocus ) {
			$strTag .= " autofocus=\"autofocus\"";
			$this->m_bInputAutoFocus = false;
		}
		if ( $this->m_bInputDisabled ) {
			$strTag .= " disabled=\"disabled\"";
			$this->m_bInputDisabled = false;
		}

		// Exclude UIs
		if ( in_array($strType, $arrUIs) ) {
			if ( $strType == "checkbox" || $strType == "radio" ) {
				if ( $this->m_bInputRequired ) {
					$strTag .= " required=\"required\"";
					$this->m_bInputRequired = false;
				}
			}
			return $strTag;
		}

		// All remaining except one
		if ( $strType != "textarea" ) {
			if ( $this->m_strInputDataList != NULL ) {
				$strTag .= " list=\"$this->m_strInputDataList\"";
				$this->m_strInputDataList = NULL;
			}
		}

		// Exclude Miscs
		if ( $strType == "color" ) {
			if ( $this->m_bInputAutoComplete != NULL ) {
				$strOnOff = $this->m_bInputAutoComplete ? "on" : "off";
				$strTag .= " autocomplete=\"$strOnOff\"";
				$this->m_bInputAutoComplete = NULL;
			}
			return $strTag;
		}
		if ( $strType == "file" ) {
			if ( $this->m_bInputRequired ) {
				$strTag .= " required=\"required\"";
				$this->m_bInputRequired = false;
			}
			return $strTag;
		}

		// All remaining except some
		if ( $strType != "range" ) {
			if ( $this->m_bInputRequired ) {
				$strTag .= " required=\"required\"";
				$this->m_bInputRequired = false;
			}
		}
		if ( $strType != "number" && $strType != "textarea" ) {
			if ( $this->m_bInputAutoComplete != NULL ) {
				$strOnOff = $this->m_bInputAutoComplete ? "on" : "off";
				$strTag .= " autocomplete=\"$strOnOff\"";
				$this->m_bInputAutoComplete = NULL;
			}
		}

		// Exclude texts
		if ( in_array($strType, $arrTexts) ) {
			if ( $this->m_iInputMaxLength != NULL ) {
				$strTag .= " maxlength=\"$this->m_iInputMaxLength\"";
				$this->m_iInputMaxLength = NULL;
			}
			if ( $this->m_strInputHint != NULL ) {
				$strTag .= " placeholder=\"$this->m_strInputHint\"";
				$this->m_strInputHint = NULL;
			}
			if ( $strType != "textarea" ) {
				if ( $this->m_iInputCharSize != NULL ) {
					$strTag .= " size=\"$this->m_iInputCharSize\"";
					$this->m_iInputCharSize = NULL;
				}
				if ( $this->m_strInputPattern != NULL ) {
					$strTag .= " pattern=\"$this->m_strInputPattern\"";
					$this->m_strInputPattern = NULL;
				}
			}
			return $strTag;
		}
		
		// Numerics or dates remaining
		if ( $this->m_iInputMin != NULL ) {
			$strTag .= " min=\"$this->m_iInputMin\"";
			$this->m_iInputMin = NULL;
		}
		if ( $this->m_iInputMax != NULL ) {
			$strTag .= " max=\"$this->m_iInputMax\"";
			$this->m_iInputMax = NULL;
		}
		if ( $this->m_iInputStep != NULL ) {
			$strTag .= " step=\"$this->m_iInputStep\"";
			$this->m_iInputStep = NULL;
		}
		return $strTag;
	}

	// Members
		// Indentation support
    private $m_bIndentDepth;
	
		// Tag attributes support
	private $m_strID;
	private $m_iTabIndex;
	private $m_strTooltip;
	private $m_strContextMenu;
	private $m_bEditableContent;
	private $m_bHidden;
	private $m_strLanguage;
	private $m_strStyleClass;
	private $m_strStyleInline;
	private $m_strDragStartFunction;
	private $m_strDragOverFunction;
	private $m_strDropFunction;
	private $m_arrEventCallbacks;
	private $m_arrCustomAttributes;

	private $m_strInputFormID;
	private $m_strInputName;
	private $m_bInputLocked;
	private $m_bInputRequired;
	private $m_bInputDisabled;
	private $m_bInputAutoFocus;
	private $m_bInputAutoComplete;
	private $m_strInputDataList;
	private $m_iInputMaxLength;
	private $m_iInputCharSize;
	private $m_strInputPattern;
	private $m_strInputHint;
	private $m_iInputMin;
	private $m_iInputMax;
	private $m_iInputStep;

		// Context tracking
    private $m_bInHTML;
    private $m_bInHead;
    private $m_bInBody;
	private $m_bInDynamicTag;

    private $m_bInStyle;
    private $m_bInScript;

	private $m_bInHeader;
	private $m_bInFooter;
	private $m_bInSection;

	private $m_bInIFrame;
    private $m_iDivDepth;

	private $m_bInList;
	private $m_bOrderedList;
	private $m_bInListItem;

	private $m_iTableDepth;
	private $m_bInTableColGroup;
	private $m_iTableHeaderDepth;
	private $m_iTableFooterDepth;
	private $m_iTableBodyDepth;
	private $m_iTableRowDepth;
	private $m_iTableHeadCellDepth;
	private $m_iTableCellDepth;

	private $m_bInSpan;

	private $m_bInArticle;
	private $m_bInAside;
	private $m_bInTitleGroup;
	private $m_bInTitle;
	private $m_iTitleLevel;
	private $m_iParagraphDepth;
	private $m_bInQuote;
	private $m_bInHighLight;
	private $m_iDetailsDepth;
	private $m_bInDefList;
	private $m_bInDefListItemName;
	private $m_bInDefListItemContent;
	private $m_bInHyperlinkGroup;
	private $m_bInHyperlink;

	private $m_bInDataList;
    private $m_bInComboList;
    private $m_bInComboListGroup;
	private $m_iMenuDepth;
	private $m_iMenuItemDepth;

    private $m_bInForm;
    private $m_iFormBlockDepth;

	private $m_bInMultimedia;
	private $m_bInImageMap;
	private $m_bInAudio;
	private $m_bInVideo;
	private $m_bInObject;
}

?>