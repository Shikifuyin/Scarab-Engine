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
require_once "HTMLTag.php";
require_once "HTMLNode.php";
require_once "HTMLLeaf.php";

require_once "HTMLDocument.php";
require_once "HTMLStructures.php";
require_once "HTMLTextFormat.php";
require_once "HTMLContent.php";
require_once "HTMLGUI.php";
require_once "HTMLInput.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/*
	// DropFunction shortcuts (future DropZone attribute)
define( "HTML_DROPFUNCTION_COPY", "copy" ); // copy dragged data
define( "HTML_DROPFUNCTION_MOVE", "move" ); // move dragged data
define( "HTML_DROPFUNCTION_LINK", "link" ); // create link to original data

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
*/

/////////////////////////////////////////////////////////////////////////////////
// The HTML2Manager class
class HTML2Manager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( HTML2Manager::$m_hInstance == NULL )
            HTML2Manager::$m_hInstance = new HTML2Manager();
        return HTML2Manager::$m_hInstance;
    }
    protected static $m_hInstance;

    // Constructor / Destructor
    public function __construct()
    {
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Media query builder
	public function BuildMediaQuery( $strDevice, $strParam, $mValue, $strPrefix = NULL, $strMergeWith = NULL ) {
		if ( $strPrefix != NULL )
			$strQuery = "$strDevice and ($strPrefix-$strParam:$mValue)";
		else
			$strQuery = "$strDevice and ($strParam:$mValue)";
		if ( $strMergeWith != NULL )
			$strQuery = "$strMergeWith, $strQuery";
		return $strQuery;
	}

	// Document Factory
	public function CreateDocument() {
		$hRoot = new HTMLDocument();
		$hRoot->AppendChild( new HTMLHead($hRoot) );
		$hRoot->AppendChild( new HTMLBody($hRoot) );
		return $hRoot;
	}

	// Main output routine
	public function OutputDocument( $hDocument ) {
		echo "<!-- HTML 5 page generated by ScarabEngine's PHP Engine ... DO NOT EDIT ! -->";
		echo "<!DOCTYPE html>";
		$hDocument->Output( 0 );
	}
}

?>