<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLDocument.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : Document Tags
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
	// Meta types
define( "HTML_HEAD_META_NAME",        "application-name" );
define( "HTML_HEAD_META_AUTHOR",      "author" );
define( "HTML_HEAD_META_DESCRIPTION", "description" );
define( "HTML_HEAD_META_KEYWORDS",    "keywords" );
define( "HTML_HEAD_META_CHARSET",     "charset" );

	// Document link targets
define( "HTML_TARGET_SELF",   "_self" );   // Same frame (default)
define( "HTML_TARGET_PARENT", "_parent" ); // Parent frame
define( "HTML_TARGET_TOP",    "_top" );    // Top level frame (full body)
define( "HTML_TARGET_BLANK",  "_blank" );  // New window or tab
//const HTML_TARGET_IFRAME             // Named <iframe>

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

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDocument class
class HTMLDocument extends HTMLNode
{
	// Constructor / Destructor
    public function __construct()
    {
		parent::__construct( "html", NULL, false );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHead class
class HTMLHead extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "head", $hParent, false );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLBody class
class HTMLBody extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "body", $hParent, false );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadTitle class
class HTMLHeadTitle extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "title", $hParent, false );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadBase class
class HTMLHeadBase extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strDefaultURL, $strDefaultTarget )
    {
		parent::__construct( "base", $hParent, false );
		
		$this->m_strDefaultURL = $strDefaultURL;
		$this->m_strDefaultTarget = $strDefaultTarget;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " href=\"" . $this->m_strDefaultURL . "\"";
		$strAttributes .= " target=\"" . $this->m_strDefaultTarget . "\"";
	}

	// Members
	private $m_strDefaultURL;
	private $m_strDefaultTarget;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadMeta class
class HTMLHeadMeta extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strType, $strContent )
    {
		parent::__construct( "meta", $hParent, false );
		
		$this->m_strType = $strType;
		$this->m_strContent = $strContent;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_strType == HTML_HEAD_META_CHARSET )
			$strAttributes .= " charset=\"" . $this->m_strContent . "\"";
		else {
			$strAttributes .= " name=\"" . $this->m_strType . "\"";
			$strAttributes .= " content=\"" . $this->m_strContent . "\"";
		}
	}

	// Members
	private $m_strType;
	private $m_strContent;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadMeta class
class HTMLHeadLink extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strFile, $strType, $strRelation, $strMediaQuery = NULL, $strIconSizes = NULL )
    {
		parent::__construct( "link", $hParent, false );

		$this->m_strFile = $strFile;		
		$this->m_strType = $strType;
		$this->m_strRelation = $strRelation;
		$this->m_strMediaQuery = $strMediaQuery;
		$this->m_strIconSizes = $strIconSizes;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " href=\"" . $this->m_strFile . "\"";
		$strAttributes .= " type=\"" . $this->m_strType . "\"";
		$strAttributes .= " rel=\"" . $this->m_strRelation . "\"";
		if ( $this->m_strMediaQuery != NULL )
			$strAttributes .= " media=\"" . $this->m_strMediaQuery . "\"";
		if ( $this->m_strRelation == HTML_RELATION_ICON && $this->m_strIconSizes != NULL )
			$strAttributes .= " sizes=\"" . $this->m_strIconSizes . "\"";
	}

	// Members
	private $m_strFile;
	private $m_strType;
	private $m_strRelation;
	private $m_strMediaQuery;
	private $m_strIconSizes;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadStyle class
class HTMLHeadStyle extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strMediaQuery = NULL )
    {
		parent::__construct( "style", $hParent, false );

		$this->m_strMediaQuery = $strMediaQuery;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"text/css\"";
		if ( $this->m_strMediaQuery != NULL )
			$strAttributes .= " media=\"" . $this->m_strMediaQuery . "\"";
	}

	// Members
	private $m_strMediaQuery;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadNoScript class
class HTMLHeadNoScript extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "noscript", $hParent, false );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadScript class
class HTMLHeadScriptInclude extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strURL, $strType, $strCharset = NULL, $bDefer = false, $bAsynchronous = false )
    {
		parent::__construct( "script", $hParent, false );
		
		$this->m_strURL = $strURL;
		$this->m_strType = $strType;
		$this->m_strCharset = $strCharset;
		$this->m_bDefer = $bDefer;
		$this->m_bAsynchronous = $bAsynchronous;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strURL . "\"";
		$strAttributes .= " type=\"" . $this->m_strType . "\"";
		if ( $this->m_strCharset != NULL )
			$strAttributes .= " charset=\"" . $this->m_strCharset . "\"";
		if ( $this->m_bDefer )
			$strAttributes .= " defer=\"defer\"";
		if ( $this->m_bAsynchronous )
			$strAttributes .= " async=\"async\"";
	}

	// Members
	private $m_strURL;
	private $m_strType;
	private $m_strCharset;
	private $m_bDefer;
	private $m_bAsynchronous;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeadScript class
class HTMLHeadScript extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strType )
    {
		parent::__construct( "script", $hParent, false );
		
		$this->m_strType = $strType;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " type=\"" . $this->m_strType . "\"";
	}

	// Members
	private $m_strType;
}

?>