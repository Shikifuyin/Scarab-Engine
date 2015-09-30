<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/CSS/CSS.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : CSS 3.0 definitions and interface
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
	// Units
define( "CSS_UNIT_PIXEL",      "px" );
define( "CSS_UNIT_CENTIMETER", "cm" );
define( "CSS_UNIT_MILLIMETER", "mm" );
define( "CSS_UNIT_INCH",       "in" );
define( "CSS_UNIT_POINT",      "pt" ); // = 1/72 inch
define( "CSS_UNIT_PICA",       "pc" ); // = 12 pt = 1/6 inch
define( "CSS_UNIT_PERCENT",    "%" );
define( "CSS_UNIT_CHARACTER",  "em" );
define( "CSS_UNIT_XHEIGHT",    "ex" );

	// Colors
define( "CSS_COLOR_TRANSPARENT", "transparent" );
/*		// Grayscale
define( "CSS_COLOR_BLACK",  array( "Name" => "black",  "Value" => "#000000" ) ); // Standard
define( "CSS_COLOR_WHITE",  array( "Name" => "white",  "Value" => "#ffffff" ) ); // Standard
define( "CSS_COLOR_SILVER", array( "Name" => "silver", "Value" => "#c0c0c0" ) ); // Standard
define( "CSS_COLOR_GRAY",   array( "Name" => "gray",   "Value" => "#808080" ) ); // Standard
define( "CSS_COLOR_GREY",   array( "Name" => "grey",   "Value" => "#808080" ) ); // Standard
		// Red
define( "CSS_COLOR_RED",    array( "Name" => "red",    "Value" => "#ff0000" ) ); // Standard
define( "CSS_COLOR_MAROON", array( "Name" => "maroon", "Value" => "#800000" ) ); // Standard
		// Yellow
define( "CSS_COLOR_YELLOW", array( "Name" => "yellow", "Value" => "#ffff00" ) ); // Standard
define( "CSS_COLOR_OLIVE",  array( "Name" => "olive",  "Value" => "#808000" ) ); // Standard
		// Green
define( "CSS_COLOR_LIME",  array( "Name" => "lime",  "Value" => "#00ff00" ) ); // Standard
define( "CSS_COLOR_GREEN", array( "Name" => "green", "Value" => "#008000" ) ); // Standard
		// Cyan
define( "CSS_COLOR_AQUA", array( "Name" => "aqua", "Value" => "#00ffff" ) ); // Standard
define( "CSS_COLOR_TEAL", array( "Name" => "teal", "Value" => "#008080" ) ); // Standard
		// Blue
define( "CSS_COLOR_BLUE", array( "Name" => "blue", "Value" => "#0000ff" ) ); // Standard
define( "CSS_COLOR_NAVY", array( "Name" => "navy", "Value" => "#000080" ) ); // Standard
		// Magenta
define( "CSS_COLOR_FUSCHIA", array( "Name" => "fuchsia", "Value" => "#ff00ff" ) ); // Standard
define( "CSS_COLOR_PURPLE",  array( "Name" => "purple",  "Value" => "#800080" ) ); // Standard
*/
	// Selectors Attribute conditions
define( "CSS_SELECTOR_ATTRIBUTE_EQUALS",   "=" );
define( "CSS_SELECTOR_ATTRIBUTE_CONTAINS", "*=" );
define( "CSS_SELECTOR_ATTRIBUTE_PREFIX",   "^=" );
define( "CSS_SELECTOR_ATTRIBUTE_SUFFIX",   "\$=" );
define( "CSS_SELECTOR_ATTRIBUTE_INLIST_SPACES", "~=" );
define( "CSS_SELECTOR_ATTRIBUTE_INLIST_DASHES", "|=" );

	// Selectors content sub-selection
define( "CSS_SELECTOR_CONTENT_FIRSTCHAR",    0x00000000 );
define( "CSS_SELECTOR_CONTENT_FIRSTLINE",    0x00000001 );
define( "CSS_SELECTOR_CONTENT_INSERTBEFORE", 0x00000002 );
define( "CSS_SELECTOR_CONTENT_INSERTAFTER",  0x00000003 );

	// Selectors node conditions
define( "CSS_SELECTOR_NODE_LEAF",        0x00000000 );
define( "CSS_SELECTOR_NODE_FIRSTCHILD",  0x00000001 );
define( "CSS_SELECTOR_NODE_LASTCHILD",   0x00000002 );
define( "CSS_SELECTOR_NODE_UNIQUECHILD", 0x00000003 );
define( "CSS_SELECTOR_NODE_NTHCHILD",    0x00000004 );

	// Selectors state conditions
define( "CSS_SELECTOR_STATE_ENABLED",   0x00000000 );
define( "CSS_SELECTOR_STATE_DISABLED",  0x00000001 );
define( "CSS_SELECTOR_STATE_CHECKED",   0x00000002 );
define( "CSS_SELECTOR_STATE_UNVISITED", 0x00000003 );
define( "CSS_SELECTOR_STATE_VISITED",   0x00000004 );

	// Selectors event conditions
define( "CSS_SELECTOR_EVENT_FOCUS",      0x00000000 );
define( "CSS_SELECTOR_EVENT_ACTIVATE",   0x00000001 );
define( "CSS_SELECTOR_EVENT_ANCHORJUMP", 0x00000002 );
define( "CSS_SELECTOR_EVENT_MOUSEOVER",  0x00000003 );

	// Selectors joint operations
define( "CSS_SELECTOR_JOINT_PARENT",   0x00000000 ); // Parent-Child relation
define( "CSS_SELECTOR_JOINT_ANCESTOR", 0x00000001 ); // Ancestor-Successor relation
define( "CSS_SELECTOR_JOINT_SIBBLING", 0x00000002 ); // Sibblings relation (adjacent)
define( "CSS_SELECTOR_JOINT_BROTHER",  0x00000003 ); // Brothers relation
define( "CSS_SELECTOR_JOINT_AND",      0x00000004 ); // Select on matching all selectors
define( "CSS_SELECTOR_JOINT_OR",       0x00000005 ); // Select on matching on any one selector

	// Properties, Global values
define( "CSS_PROPERTY_INITIAL", "initial" );
define( "CSS_PROPERTY_INHERIT", "inherit" );

	// Properties, Background
define( "CSS_BACKGROUND_SCROLL_NORMAL",        "scroll" );      // default
define( "CSS_BACKGROUND_SCROLL_FIXED",         "fixed" );
define( "CSS_BACKGROUND_IMAGE_NONE",           "none" );        // default
define( "CSS_BACKGROUND_ORIGIN_BORDER",        "border-box" );
define( "CSS_BACKGROUND_ORIGIN_PADDING",       "padding-box" ); // default
define( "CSS_BACKGROUND_ORIGIN_CONTENT",       "content-box" );
define( "CSS_BACKGROUND_POSITION_CENTER",      "center" );      // (0,0), default
define( "CSS_BACKGROUND_POSITION_LEFT",        "left" );        // (0,y)
define( "CSS_BACKGROUND_POSITION_RIGHT",       "right" );       // (W-ImgW,y)
define( "CSS_BACKGROUND_POSITION_TOP",         "top" );         // (x,0)
define( "CSS_BACKGROUND_POSITION_BOTTOM",      "bottom" );      // (x,H-ImgH)
define( "CSS_BACKGROUND_SIZE_AUTO",            "auto" );        // default
define( "CSS_BACKGROUND_SIZE_COVER",           "cover" );
define( "CSS_BACKGROUND_SIZE_CONTAIN",         "contain" );
define( "CSS_BACKGROUND_REPEAT_BOTH",          "repeat" );      // default
define( "CSS_BACKGROUND_REPEAT_HORIZ",         "repeat-x" );
define( "CSS_BACKGROUND_REPEAT_VERT",          "repeat-y" );
define( "CSS_BACKGROUND_REPEAT_NONE",          "no-repeat" );
define( "CSS_BACKGROUND_CLIP_BORDER",          "border-box" );  // default
define( "CSS_BACKGROUND_CLIP_PADDING",         "padding-box" );
define( "CSS_BACKGROUND_CLIP_CONTENT",         "content-box" );

/////////////////////////////////////////////////////////////////////////////////
// The CSSManager class
class CSSManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( CSSManager::$m_hInstance == NULL )
            CSSManager::$m_hInstance = new CSSManager();
        return CSSManager::$m_hInstance;
    }
    protected static $m_hInstance;

    // Constructor / Destructor
    public function __construct()
    {
		$hSystem = SystemManager::GetSingleton();
		$this->m_arrClientBrowser = $hSystem->GetClientBrowser();

		$this->m_strDefaultUnit = CSS_UNIT_PIXEL;

		$this->m_bInDeclaration = false;
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Units
	public function MakeUnitValue( $iValue, $strUnit = CSS_UNIT_PIXEL ) {
		return "$iValue$strUnit";
	}

	// Colors
	public function MakeColorHex( $iRed, $iGreen, $iBlue ) {
		$strRed = dechex( $iRed );
		$strGreen = dechex( $iGreen );
		$strBlue = dechex( $iBlue );
		return "#$strRed$strGreen$strBlue";
	}
	public function MakeColorRGBA( $iRed, $iGreen, $iBlue, $iAlpha = NULL ) {
		if ( $iAlpha != NULL )
			return "rgba($iRed,$iGreen,$iBlue,$iAlpha)";
		else
			return "rgb($iRed,$iGreen,$iBlue)";
	}
	public function MakeColorHSLA( $iHue, $iSaturation, $iLightness, $iAlpha = NULL ) {
		if ( $iAlpha != NULL )
			return "hsla($iHue,$iSaturation%,$iLightness%,$iAlpha)";
		else
			return "hsl($iHue,$iSaturation%,$iLightness%)";
	}

	public function GetColorConstant( $strColorName ) {
		return constant( "CSS_COLOR_" . strtoupper($strName) );
	}
	public function MakeColorFromName( $strColorName ) {
		$arrConstant = constant( "CSS_COLOR_" . strtoupper($strName) );
		return $arrConstant["Value"];
	}

	// Resources
	public function MakeResource( $strURL ) {
		return "url('$strURL')";
	}

	// Selectors
	public function SelectorAll() {
		return "*";
	}
	public function SelectorRoot() {
		return ":root";
	}
	public function SelectorType( $strTag ) {
		return "$strTag";
	}

	public function SelectorID( $strID ) {
		return "#$strID";
	}
	public function SelectorClass( $strClass ) {
		return ".$strClass";
	}
	public function SelectorAttribute( $strAttribute, $strCondition = NULL, $strValue = "" ) {
		if ( $strCondition == NULL )
			return "[$strAttribute]";
		assert( '$strValue != ""' );
		return "[$strAttribute$strCondition$strValue]";
	}

// DEPRECATED
//	public function SelectorSelection() {
//		return ( $this->_IsFirefox() ? "::-moz-selection" : "::selection" );
//	}

	public function SelectorContent( $strSelector, $iSubSelect )
	{
		switch( $iSubSelect ) {
			case CSS_SELECTOR_CONTENT_FIRSTCHAR:    return $strSelector . ":first-letter";
			case CSS_SELECTOR_CONTENT_FIRSTLINE:    return $strSelector . ":first-line";
			case CSS_SELECTOR_CONTENT_INSERTBEFORE: return $strSelector . ":before";
			case CSS_SELECTOR_CONTENT_INSERTAFTER:  return $strSelector . ":after";
			default: assert('false'); return NULL;
		}
	}

	public function SelectorConditionLanguage( $strSelector, $strLanguage ) {
		return $strSelector . ":lang($strLanguage)";
	}
	public function SelectorConditionNode( $strSelector, $iNodeCondition, $bTypeRestrict = false, $strIndexExpr = NULL, $bReversed = false )
	{
		switch( $iNodeCondition ) {
			case CSS_SELECTOR_NODE_LEAF:        return $strSelector . ":empty";
			case CSS_SELECTOR_NODE_FIRSTCHILD:  return $strSelector . ( $bTypeRestrict ? ":first-of-type" : ":first-child" );
			case CSS_SELECTOR_NODE_LASTCHILD:   return $strSelector . ( $bTypeRestrict ? ":last-of-type" : ":last-child" );
			case CSS_SELECTOR_NODE_UNIQUECHILD: return $strSelector . ( $bTypeRestrict ? ":only-of-type" : ":only-child" );
			case CSS_SELECTOR_NODE_NTHCHILD:
				assert( '$strFormulaIndex != NULL' );
				if ( $bReversed )
					return $strSelector . ( $bTypeRestrict ? ":nth-last-of-type($strIndexExpr)" : ":nth-last-child($strIndexExpr)" );
				else
					return $strSelector . ( $bTypeRestrict ? ":nth-of-type($strIndexExpr)" : ":nth-child($strIndexExpr)" );
			default: assert('false'); return NULL;
		}
	}
	public function SelectorConditionState( $iStateCondition )
	{
		switch( $iStateCondition ) {
			case CSS_SELECTOR_STATE_UNVISITED: return ":link";
			case CSS_SELECTOR_STATE_VISITED:   return ":visited";
			case CSS_SELECTOR_STATE_ENABLED:   return ":enabled";
			case CSS_SELECTOR_STATE_DISABLED:  return ":disabled";
			case CSS_SELECTOR_STATE_CHECKED:   return ":checked";
			default: assert('false'); return NULL;
		}
	}
	public function SelectorConditionEvent( $iEventCondition )
	{
		switch( $iEventCondition ) {
			case CSS_SELECTOR_EVENT_FOCUS:      return ":focus";
			case CSS_SELECTOR_EVENT_MOUSEOVER:  return ":hover";
			case CSS_SELECTOR_EVENT_ACTIVATE:   return ":active";
			case CSS_SELECTOR_EVENT_ANCHORJUMP: return ":target";
			default: assert('false'); return NULL;
		}
	}

	public function SelectorNot( $strSelector ) {
		return ":not($strSelector)";
	}
	public function SelectorJoint( $arrSelectors, $iJointOperation )
	{
		switch( $iJointOperation ) {
			case CSS_SELECTOR_JOINT_PARENT:   return implode( ">", $arrSelectors );
			case CSS_SELECTOR_JOINT_ANCESTOR: return implode( " ", $arrSelectors );
			case CSS_SELECTOR_JOINT_SIBBLING: return implode( "+", $arrSelectors );
			case CSS_SELECTOR_JOINT_BROTHER:  return implode( "~", $arrSelectors );
			case CSS_SELECTOR_JOINT_AND:      return implode(  "", $arrSelectors );
			case CSS_SELECTOR_JOINT_OR:       return implode( ",", $arrSelectors );
			default: assert('false'); return NULL;
		}
	}

	// Declaration struture
	public function DeclarationStart( $strSelector )
	{
		assert( '!($this->m_bInDeclaration)' );
		
		$hHTML = HTMLManager::GetSingleton();
		
		$hHTML->HeadStyleCode( "$strSelector {" );

		$this->m_bInDeclaration = true;
	}
	public function DeclarationEnd()
	{
		assert( '$this->m_bInDeclaration' );

		$hHTML = HTMLManager::GetSingleton();

		$hHTML->HeadStyleCode( "}" );

		$this->m_bInDeclaration = false;
	}

	// Properties

//	// Properties, Border
//define( "CSS_BORDER_LEFT",         "left" );
//define( "CSS_BORDER_RIGHT",        "right" );
//define( "CSS_BORDER_TOP",          "top" );
//define( "CSS_BORDER_BOTTOM",       "bottom" );
//define( "CSS_BORDER_SIZE_THIN",    "thin" );
//define( "CSS_BORDER_SIZE_MEDIUM",  "medium" ); // default
//define( "CSS_BORDER_SIZE_THICK",   "thick" );
//define( "CSS_BORDER_STYLE_NONE",   "none" );    // default (enforced)
//define( "CSS_BORDER_STYLE_HIDDEN", "hidden" );
//define( "CSS_BORDER_STYLE_", "" );
//
//none 	Specifies no border 	Play it »	
//hidden 	The same as "none", except in border conflict resolution for table elements 	Play it »	
//dotted 	Specifies a dotted border 	Play it »	
//dashed 	Specifies a dashed border 	Play it »
//solid 	Specifies a solid border 	Play it »
//double 	Specifies a double border 	Play it »
//groove 	Specifies a 3D grooved border. The effect depends on the border-color value 	Play it »
//ridge 	Specifies a 3D ridged border. The effect depends on the border-color value 	Play it »
//inset 	Specifies a 3D inset border. The effect depends on the border-color value 	Play it »
//outset 	Specifies a 3D outset border. The effect depends on the border-color value 	Play it »
//inherit
//	// Properties, Outline
//define( "CSS_OUTLINE_", "" ); // default

		// Border properties
	public function PropertyBorderSize( $strBorder, $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tborder-$strBorder-width:$strValue;" );
	}
	public function PropertyBorderColor( $strBorder, $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tborder-$strBorder-color:$strValue;" );
	}
	public function PropertyBorderStyle( $strBorder, $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tborder-$strBorder-style:$strValue;" );
	}
		// Outline properties
	public function PropertyOutlineSize( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\toutline-width:$strValue;" );
	}
	public function PropertyOutlineColor( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\toutline-color:$strValue;" );
	}
	public function PropertyOutlineStyle( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\toutline-style:$strValue;" );
	}

		// Background
	public function PropertyBackgroundScroll( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-attachment:$strValue;" );
	}
	public function PropertyBackgroundColor( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-color:$strValue;" );
	}
	public function PropertyBackgroundImage( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-image:$strValue;" );
	}
	public function PropertyBackgroundOrigin( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-origin:$strValue;" );
	}
	public function PropertyBackgroundPosition( $strValueX, $strValueY = "" ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-position:$strValueX $strValueY;" );
	}
	public function PropertyBackgroundSize( $strValueX, $strValueY = "" ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-size:$strValueX $strValueY;" );
	}
	public function PropertyBackgroundRepeat( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		$hHTML->HeadStyleCode( "\tbackground-repeat:$strValue;" );
	}
	public function PropertyBackgroundClip( $strValue ) {
		assert( '$this->m_bInDeclaration' );
		if ( $this->_IsSafari() )
			$hHTML->HeadStyleCode( "\t-webkit-background-clip:$strValue;" );
		else
			$hHTML->HeadStyleCode( "\tbackground-clip:$strValue;" );
	}

	// Position properties
	// Dimension properties
	// Box properties
	// LineBox properties
	// FlexibleBox properties
	// Multicolumn properties
	// Grid properties
	
	// UI properties

	// Border/Outline properties
	// Margin properties
	// Padding properties
	// Color properties

	// Text properties
	// Font properties

	// HyperLink properties
	// List properties
	// Table properties

	// Paged properties
	// ContentPaged properties
	
	// GeneratedContent properties

	// Transform properties

	// Animation properties
	// Transition properties
	// Marquee properties

	// Print properties
	// Speech properties
	// Ruby properties

	// Helpers
	private function _IsFirefox() {
		return ( $this->m_arrClientBrowser["Name"] == SYSTEM_BROWSER_FIREFOX );
	}
	private function _IsIE() {
		return ( $this->m_arrClientBrowser["Name"] == SYSTEM_BROWSER_IE );
	}
	private function _IsChrome() {
		return ( $this->m_arrClientBrowser["Name"] == SYSTEM_BROWSER_CHROME );
	}
	private function _IsOpera() {
		return ( $this->m_arrClientBrowser["Name"] == SYSTEM_BROWSER_OPERA );
	}
	private function _IsSafari() {
		return ( $this->m_arrClientBrowser["Name"] == SYSTEM_BROWSER_SAFARI );
	}

	// Members
	private $m_arrClientBrowser;

	private $m_bInDeclaration;
}
?>