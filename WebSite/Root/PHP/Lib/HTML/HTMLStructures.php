<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/HTML/HTMLStrutures.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTML 5.0 : Strutural Tags
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

/////////////////////////////////////////////////////////////////////////////////
// The HTMLHeader class
class HTMLHeader extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "header", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLFooter class
class HTMLFooter extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "footer", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLSection class
class HTMLSection extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "section", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLIFrame class
class HTMLIFrame extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $strName, $strSourceURL, $iWidth = NULL, $iHeight = NULL, $bNaked = false, $bSourceCode = false )
    {
		parent::__construct( "iframe", $hParent );
		
		$this->m_strName = $strName;
		$this->m_strSourceURL = $strSourceURL;
		$this->m_iWidth = $iWidth;
		$this->m_iHeight = $iHeight;
		$this->m_bNaked = $bNaked;
		$this->m_bSourceCode = $bSourceCode;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		$strAttributes .= " name=\"" . $this->m_strName . "\"";
		if ( $this->m_bSourceCode )
			$strAttributes .= " srcdoc=\"" . $this->m_strSourceURL . "\"";
		else
			$strAttributes .= " src=\"" . $this->m_strSourceURL . "\"";
		if ( $this->m_iWidth != NULL )
			$strAttributes .= " width=\"" . $this->m_iWidth . "\"";
		if ( $this->m_iHeight != NULL )
			$strAttributes .= " height=\"" . $this->m_iHeight . "\"";
		if ( $this->m_bNaked )
			$strAttributes .= " seamless=\"seamless\"";
	}

	// Members
	private $m_strName;
	private $m_strSourceURL;
	private $m_iWidth;
	private $m_iHeight;
	private $m_bNaked;
	private $m_bSourceCode;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLDiv class
class HTMLDiv extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "div", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLList class
class HTMLList extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $bOrdered = false, $strMarkerType = NULL, $iStart = NULL, $bReversed = false )
    {
		parent::__construct( $bOrdered ? "ol" : "ul", $hParent );
		
		$this->m_bOrdered = $bOrdered;
		$this->m_strMarkerType = $strMarkerType;
		$this->m_iStart = $iStart;
		$this->m_bReversed = $bReversed;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_bOrdered ) {
			if ( $this->m_strMarkerType != NULL )
				$strAttributes .= " type=\"" . $this->m_strMarkerType . "\"";
			if ( $this->m_iStart != NULL )
				$strAttributes .= " start=\"" . $this->m_iStart . "\"";
			if ( $this->m_bReversed )
				$strAttributes .= " reversed=\"reversed\"";
		}
	}

	// Members
	private $m_bOrdered;
	private $m_strMarkerType;
	private $m_iStart;
	private $m_bReversed;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLListItem class
class HTMLListItem extends HTMLNode
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
// The HTMLTable class
class HTMLTable extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $bCellBorders = false )
    {
		parent::__construct( "table", $hParent );
		
		$this->m_bCellBorders = $bCellBorders;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_bCellBorders )
			$strAttributes .= " border=\"1\"";
	}

	// Members
	private $m_bCellBorders;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableCaption class
class HTMLTableCaption extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "caption", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableColumnGroup class
class HTMLTableColumnGroup extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "colgroup", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableColumnGroupItem class
class HTMLTableColumnGroupItem extends HTMLLeaf
{
	// Constructor / Destructor
    public function __construct( $hParent, $iSpan = NULL )
    {
		parent::__construct( "col", $hParent );
		
		$this->m_iSpan = $iSpan;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_iSpan != NULL )
			$strAttributes .= " span=\"" . $this->m_iSpan . "\"";
	}

	// Members
	private $m_iSpan;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableHeader class
class HTMLTableHeader extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "thead", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableFooter class
class HTMLTableFooter extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "tfoot", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableBody class
class HTMLTableBody extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "tbody", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableRow class
class HTMLTableRow extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "tr", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableHeadCell class
class HTMLTableHeadCell extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iColumnSpan = NULL, $iRowSpan = NULL, $strScope = NULL, $strHeaderIDs = NULL )
    {
		parent::__construct( "th", $hParent );
		
		$this->m_iColumnSpan = $iColumnSpan;
		$this->m_iRowSpan = $iRowSpan;
		$this->m_strScope = $strScope;
		$this->m_strHeaderIDs = $strHeaderIDs;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_iColumnSpan != NULL )
			$strAttributes .= " colspan=\"" . $this->m_iColumnSpan . "\"";
		if ( $this->m_iRowSpan != NULL )
			$strAttributes .= " rowspan=\"" . $this->m_iRowSpan . "\"";
		if ( $this->m_strScope != NULL )
			$strAttributes .= " scope=\"" . $this->m_strScope . "\"";
		if ( $this->m_strHeaderIDs != NULL )
			$strAttributes .= " headers=\"" . $this->m_strHeaderIDs . "\"";
	}

	// Members
	private $m_iColumnSpan;
	private $m_iRowSpan;
	private $m_strScope;
	private $m_strHeaderIDs;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLTableCell class
class HTMLTableCell extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent, $iColumnSpan = NULL, $iRowSpan = NULL, $strHeaderIDs = NULL )
    {
		parent::__construct( "td", $hParent );
		
		$this->m_iColumnSpan = $iColumnSpan;
		$this->m_iRowSpan = $iRowSpan;
		$this->m_strHeaderIDs = $strHeaderIDs;
	}
	public function __destruct()
    {
		parent::__destruct();
	}

	// Callbacks
	protected function _Output_Attributes_Callback( & $strAttributes ) {
		if ( $this->m_iColumnSpan != NULL )
			$strAttributes .= " colspan=\"" . $this->m_iColumnSpan . "\"";
		if ( $this->m_iRowSpan != NULL )
			$strAttributes .= " rowspan=\"" . $this->m_iRowSpan . "\"";
		if ( $this->m_strHeaderIDs != NULL )
			$strAttributes .= " headers=\"" . $this->m_strHeaderIDs . "\"";
	}

	// Members
	private $m_iColumnSpan;
	private $m_iRowSpan;
	private $m_strHeaderIDs;
}

/////////////////////////////////////////////////////////////////////////////////
// The HTMLSpan class
class HTMLSpan extends HTMLNode
{
	// Constructor / Destructor
    public function __construct( $hParent )
    {
		parent::__construct( "span", $hParent );
	}
	public function __destruct()
    {
		parent::__destruct();
	}
}

?>