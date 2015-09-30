<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLLeaf.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : Leaf Tags
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

/////////////////////////////////////////////////////////////////////////////////
// The HTMLLeaf class
class HTMLLeaf extends HTMLTag
{
	// Constructor / Destructor
    public function __construct( $strTag, $hParent, $bAllowAttributes = true )
    {
		parent::__construct( $strTag, $hParent, $bAllowAttributes );
	}
	public function __destruct()
    {
		parent::__destruct();
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

	protected function _Recursive_Output( $strIndent )
	{
		// Leaf tag
		$strLeafTag = "<" . $this->m_strTag;
		if ( $this->m_bAllowAttributes )
			$this->_ApplyGlobalAttributes( $strLeafTag );
		$this->_Output_Attributes_Callback( $strLeafTag );
		$strLeafTag .= " />";
		echo "$strIndent$strLeafTag";
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLInputLeaf class
class HTMLInputLeaf extends HTMLInputTag
{
	// Constructor / Destructor
    public function __construct( $strTag, $strInputType, $hParent, $bAllowAttributes = true )
    {
		parent::__construct( $strTag, $strInputType, $hParent, $bAllowAttributes );
	}
	public function __destruct()
    {
		parent::__destruct();
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

	protected function _Recursive_Output( $strIndent )
	{
		// Leaf tag
		$strLeafTag = "<" . $this->m_strTag;
		if ( $this->m_bAllowAttributes ) {
			$this->_ApplyGlobalAttributes( $strLeafTag );
			$this->_ApplyInputAttributes( $strLeafTag );
		}
		$this->_Output_Attributes_Callback( $strLeafTag );
		$strLeafTag .= " />";
		echo "$strIndent$strLeafTag";
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLText class
class HTMLText
{
	// Constructor / Destructor
    public function __construct( $strTag, $hParent, $strText )
    {
		parent::__construct( $strTag );

		$this->m_hParent = $hParent;
		$this->m_strText = $strText;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Parent
	public function GetParent() {
		return $this->m_hParent;
	}
	public function SetParent( $hParent ) {
		$this->m_hParent = $hParent;
	}
	
	// Text
	public function GetText() {
		return $this->m_strText;
	}
	public function SetText( $strText ) {
		$this->m_strText = $strText;
	}

	// Output routine
	public function Output( $iIndentDepth = 0 ) {
		$strIndent = $this->_GetIndentString( $iIndentDepth );
		$this->_Recursive_Output( $strIndent );
	}

	// Helpers
	public function _Recursive_Output( $strIndent )
	{
		// Plain text
		$strText = $this->m_strText;
		echo "$strIndent$strText";
	}

	// Members
	protected $m_hParent;
	protected $m_strText;
}

/*

ADD FLOATING SPANS !!!

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

		// Auto-break
		if ( $iAutoBreak != NULL && $iAutoBreak > 0 )
			$strText = wordwrap( $strText, $iAutoBreak, "$strIndent\t<br/>" );

		if ( $strOpen != "" )
			echo "$strIndent$strOpen";
		echo "$strIndent\t$strText";
		if ( $strClose != "" )
			echo "$strIndent$strClose";
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
*/

?>