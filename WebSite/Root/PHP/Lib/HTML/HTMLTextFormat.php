<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLTextFormat.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : Text Formatting Tags
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
// The HTMLArticle class
class HTMLArticle extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "article", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLAside class
class HTMLAside extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "aside", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTitleGroup class
class HTMLTitleGroup extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "hgroup", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTitle class
class HTMLTitle extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iLevel )
    {
		assert( '($iLevel >= 1) && ($iLevel <= 6)' );

		parent::__construct( "h$iLevel", $hParent );
		
		$this->m_iLevel = $iLevel;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Members
	private $m_iLevel;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLParagraph class
class HTMLParagraph extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "p", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLQuote class
class HTMLQuote extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strQuotedURL = NULL )
    {
		parent::__construct( "blockquote", $hParent );
		
		$this->m_strQuotedURL = $strQuotedURL;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_strQuotedURL != NULL )
			$strAttributes .= " cite=\"" . $this->m_strQuotedURL . "\"";
	}

	// Members
	private $m_strQuotedURL;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHighLight class
class HTMLHighLight extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "mark", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDetails class
class HTMLDetails extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $bOpen = false )
    {
		parent::__construct( "details", $hParent );
		
		$this->m_bOpen = $bOpen;
	}
	public function __destruct()
    {
		parent::__destruct();
	}
	
	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_bOpen )
			$strAttributes .= " open=\"open\"";
	}

	// Members
	private $m_bOpen;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDetailsCaption class
class HTMLDetailsCaption extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "summary", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDefinitionList class
class HTMLDefinitionList extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "dl", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDefinitionListItemName class
class HTMLDefinitionListItemName extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "dt", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDefinitionListItemContent class
class HTMLDefinitionListItemContent extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "dd", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHyperLinkGroup class
class HTMLHyperLinkGroup extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "nav", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHyperLink class
class HTMLHyperLink extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strURL, $strType = NULL, $strTarget = NULL, $strRelation = NULL, $strMediaQuery = NULL )
    {
		parent::__construct( "a", $hParent );
		
		$this->m_strURL = $strURL;
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
		$strAttributes .= " href=\"" . $this->m_strURL . "\"";
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
	private $m_strURL;
	private $m_strType;
	private $m_strTarget;
	private $m_strRelation;
	private $m_strMediaQuery;
}

?>