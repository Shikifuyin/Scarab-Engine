<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLContent.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : Content Tags
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
// The HTMLDateTime class
class HTMLDateTime extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strDateTime, $bIsPublicationDate = false )
    {
		parent::__construct( "time", $hParent );
		
		$this->m_strDateTime = $strDateTime;
		$this->m_bIsPublicationDate = $bIsPublicationDate;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " datetime=\"" . $this->m_strDateTime . "\"";
		if ( $this->m_bIsPublicationDate )
			$strAttributes .= " pubdate=\"pubdate\"";
	}

	// Members
	private $m_strDateTime;
	private $m_bIsPublicationDate;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLMultimedia class
class HTMLMultimedia extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "figure", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLMultimediaCaption class
class HTMLMultimediaCaption extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "figcaption", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLImage class
class HTMLImage extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strImageSource, $strAltText = "Image", $iWidth = NULL, $iHeight = NULL,
						   				   $bIsServerMap = false, $strClientMap = NULL )
    {
		parent::__construct( "img", $hParent );
		
		$this->m_strImageSource = $strImageSource;
		$this->m_strAltText = $strAltText;
		$this->m_iWidth = $iWidth;
		$this->m_iHeight = $iHeight;
		$this->m_bIsServerMap = $bIsServerMap;
		$this->m_strClientMap = $strClientMap;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strImageSource . "\"";
		$strAttributes .= " alt=\"" . $this->m_strAltText . "\"";
		if ( $this->m_iWidth != NULL )
			$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		if ( $this->m_iHeight != NULL )
			$strAttributes .= " height=\"" . $this->m_iHeight . "\"";
		if ( $this->m_bIsServerMap )
			$strAttributes .= " ismap=\"ismap\"";
		if ( $this->m_strClientMap != NULL )
			$strAttributes .= " usemap=\"#" . $this->m_strClientMap . "\"";
	}

	// Members
	private $m_strImageSource;
	private $m_strAltText;
	private $m_iWidth;
	private $m_iHeight;
	private $m_bIsServerMap;
	private $m_strClientMap;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLImageMap class
class HTMLImageMap extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName )
    {
		parent::__construct( "map", $hParent );
		
		$this->m_strName = $strName;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
	}

	// Members
	private $m_strName;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLImageMapArea class
class HTMLImageMapArea extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strShape, $strCoords, $strURL, $strAltText = "Area", $strType = NULL, $strTarget = NULL,
							      		   $strRelation = NULL, $strMediaQuery = NULL )
    {
		parent::__construct( "area", $hParent );
		
		$this->m_strShape = $strShape;
		$this->m_strCoords = $strCoords;
		$this->m_strURL = $strURL;
		$this->m_strAltText = $strAltText;
		$this->m_strType = $strType;
		$this->m_strTarget = $strTarget;
		$this->m_strRelation = $strRelation;
		$this->m_strMediaQuery = $strMediaQuery;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " shape=\"" . $this->m_strShape . "\"";
		$strAttributes .= " coords=\"" . $this->m_strCoords . "\"";
		$strAttributes .= " href=\"" . $this->m_strURL . "\"";
		$strAttributes .= " alt=\"" . $this->m_strAltText . "\"";
		if ( $this->m_strType != NULL )
			$strAttributes .= " type=\"" . $this->m_strType . "\"";
		if ( $this->m_strTarget != NULL )
			$strAttributes .= " target=\"" . $this->m_strTarget . "\"";
		if ( $this->m_strRelation != NULL )
			$strAttributes .= " rel=\"" . $this->m_strRelation . "\"";
		if ( $this->m_strMediaQuery != NULL )
			$strAttributes .= " media=\"" . $this->m_strMediaQuery . "\"";
	}

	// Members
	private $m_strShape;
	private $m_strCoords;
	private $m_strURL;
	private $m_strAltText;
	private $m_strType;
	private $m_strTarget;
	private $m_strRelation;
	private $m_strMediaQuery;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLAudio class
class HTMLAudio extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strSourceURL, $bShowControls = false, $bAutoPlay = false, $bLoop = false, $strPreload = NULL )
    {
		parent::__construct( "audio", $hParent );
		
		$this->m_strSourceURL = $strSourceURL;
		$this->m_bShowControls = $bShowControls;
		$this->m_bAutoPlay = $bAutoPlay;
		$this->m_bLoop = $bLoop;
		$this->m_strPreload = $strPreload;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		if ( $this->m_bShowControls )
			$strAttributes .= " controls=\"controls\"";
		if ( $this->m_bAutoPlay )
			$strAttributes .= " autoplay=\"autoplay\"";
		if ( $this->m_bLoop )
			$strAttributes .= " loop=\"loop\"";
		if ( $this->m_strPreload != NULL )
			$strAttributes .= " preload=\"" . $this->m_strPreload . "\"";
	}

	// Members
	private $m_strSourceURL;
	private $m_bShowControls;
	private $m_bAutoPlay;
	private $m_bLoop;
	private $m_strPreload;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLVideo class
class HTMLVideo extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strSourceURL, $strSplashURL = NULL, $iWidth = NULL, $iHeight = NULL, $bShowControls = false,
										   $bAutoPlay = false, $bLoop = false, $bMute = false, $strPreload = NULL )
    {
		parent::__construct( "video", $hParent );
		
		$this->m_strSourceURL = $strSourceURL;
		$this->m_strSplashURL = $strSplashURL;
		$this->m_iWidth = $iWidth;
		$this->m_iHeight = $iHeight;
		$this->m_bShowControls = $bShowControls;
		$this->m_bAutoPlay = $bAutoPlay;
		$this->m_bLoop = $bLoop;
		$this->m_bMute = $bMute;
		$this->m_strPreload = $strPreload;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		if ( $this->m_strSplashURL != NULL )
			$strAttributes .= " poster=\"" . $this->m_strSplashURL . "\"";
		if ( $this->m_iWidth != NULL )
			$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		if ( $this->m_iHeight != NULL )
			$strAttributes .= " height=\"" . $this->m_iHeight . "\"";
		if ( $this->m_bShowControls )
			$strAttributes .= " controls=\"controls\"";
		if ( $this->m_bAutoPlay )
			$strAttributes .= " autoplay=\"autoplay\"";
		if ( $this->m_bLoop )
			$strAttributes .= " loop=\"loop\"";
		if ( $this->m_bMute )
			$strAttributes .= " muted=\"muted\"";
		if ( $this->m_strPreload != NULL )
			$strAttributes .= " preload=\"" . $this->m_strPreload . "\"";
	}

	// Members
	private $m_strSourceURL;
	private $m_strSplashURL;
	private $m_iWidth;
	private $m_iHeight;
	private $m_bShowControls;
	private $m_bAutoPlay;
	private $m_bLoop;
	private $m_bMute;
	private $m_strPreload;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLAudioVideoSource class
class HTMLAudioVideoSource extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strSourceURL, $strType = NULL, $strMediaQuery = NULL )
    {
		parent::__construct( "source", $hParent );
		
		$this->m_strSourceURL = $strSourceURL;
		$this->m_strType = $strType;
		$this->m_strMediaQuery = $strMediaQuery;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		if ( $this->m_strType != NULL )
			$strAttributes .= " type=\"" . $this->m_strType . "\"";
		if ( $this->m_strTarget != NULL )
			$strAttributes .= " media=\"" . $this->m_strMediaQuery . "\"";
	}

	// Members
	private $m_strSourceURL;
	private $m_strType;
	private $m_strMediaQuery;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLAudioVideoTrack class
class HTMLAudioVideoTrack extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strSourceURL, $strKind, $strLabel = NULL, $bDefault = false, $strLanguage = NULL )
    {
		parent::__construct( "track", $hParent );
		
		$this->m_strSourceURL = $strSourceURL;
		$this->m_strKind = $strKind;
		$this->m_strLabel = $strLabel;
		$this->m_bDefault = $bDefault;
		$this->m_strLanguage = $strLanguage;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		$strAttributes .= " kind=\"" . $this->m_strKind . "\"";
		if ( $this->m_strLabel != NULL )
			$strAttributes .= " label=\"" . $this->m_strLabel . "\"";
		if ( $this->m_bDefault )
			$strAttributes .= " default=\"default\"";
		if ( $this->m_strLanguage != NULL )
			$strAttributes .= " srclang=\"" . $this->m_strLanguage . "\"";
	}

	// Members
	private $m_strSourceURL;
	private $m_strKind;
	private $m_strLabel;
	private $m_bDefault;
	private $m_strLanguage;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLEmbedded class
class HTMLEmbedded extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strSourceURL, $iWidth = NULL, $iHeight = NULL, $strType = NULL )
    {
		parent::__construct( "embed", $hParent );
		
		$this->m_strSourceURL = $strSourceURL;
		$this->m_iWidth = $iWidth;
		$this->m_iHeight = $iHeight;
		$this->m_strType = $strType;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		if ( $this->m_iWidth != NULL )
			$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		if ( $this->m_iHeight )
			$strAttributes .= " height=\"" . $this->m_iHeight . "\"";
		if ( $this->m_strType != NULL )
			$strAttributes .= " type=\"" . $this->m_strType . "\"";
	}

	// Members
	private $m_strSourceURL;
	private $m_iWidth;
	private $m_iHeight;
	private $m_strType;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLObject class
class HTMLObject extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName, $strSourceURL, $iWidth, $iHeight, $strType = NULL, $strClientMap = NULL )
    {
		parent::__construct( "object", $hParent );
		
		$this->m_strName = $strName;
		$this->m_strSourceURL = $strSourceURL;
		$this->m_iWidth = $iWidth;
		$this->m_iHeight = $iHeight;
		$this->m_strType = $strType;
		$this->m_strClientMap = $strClientMap;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
		$strAttributes .= " data=\"" . $this->m_strSourceURL . "\"";
		$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		$strAttributes .= " height=\"" . $this->m_iHeight . "\"";
		if ( $this->m_strType != NULL )
			$strAttributes .= " type=\"" . $this->m_strType . "\"";
		if ( $this->m_strClientMap != NULL )
			$strAttributes .= " usemap=\"#" . $this->m_strClientMap . "\"";
	}

	// Members
	private $m_strName;
	private $m_strSourceURL;
	private $m_iWidth;
	private $m_iHeight;
	private $m_strType;
	private $m_strClientMap;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLObjectParameter class
class HTMLObjectParameter extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName, $strValue )
    {
		parent::__construct( "param", $hParent );
		
		$this->m_strName = $strName;
		$this->m_strValue = $strValue;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
		$strAttributes .= " value=\"" . $this->m_strValue . "\"";
	}

	// Members
	private $m_strName;
	private $m_strValue;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLCanvas class
class HTMLCanvas extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iWidth = NULL, $iHeight = NULL )
    {
		parent::__construct( "canvas", $hParent );
		
		$this->m_iWidth = $iWidth;
		$this->m_iHeight = $iHeight;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_iWidth != NULL )
			$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		if ( $this->m_iHeight != NULL )
			$strAttributes .= " height=\"" . $this->m_iHeight . "\"";
	}

	// Members
	private $m_iWidth;
	private $m_iHeight;
}

?>