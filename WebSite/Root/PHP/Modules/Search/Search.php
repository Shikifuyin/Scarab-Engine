<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Search/Search.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Modules, Search
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

	// Search options
define( "SEARCH_OPTION_MESSAGES",    0x00000001 );
define( "SEARCH_OPTION_DIRECTORIES", 0x00010000 );
define( "SEARCH_OPTION_FILES",       0x00020000 );
define( "SEARCH_OPTION_DEFINES",     0x00040000 );
define( "SEARCH_OPTION_MACROS",      0x00080000 );
define( "SEARCH_OPTION_NAMESPACES",  0x00100000 );
define( "SEARCH_OPTION_TYPES",       0x00200000 );
define( "SEARCH_OPTION_ENUMS",       0x00400000 );
define( "SEARCH_OPTION_TYPEDEFS",    0x00800000 );
define( "SEARCH_OPTION_VARIABLES",   0x01000000 );
define( "SEARCH_OPTION_FUNCTIONS",   0x02000000 );

define( "SEARCH_OPTION_FORUM",   0x00000001 );
define( "SEARCH_OPTION_CODEDOC", 0x03ff0000 );
define( "SEARCH_OPTION_ALL",     0x03ff0001 );

	// Search events
define( "SEARCH_DISPLAY_RESULTS", "DisplayResults" );

//define( "SEARCH_EVENT_", "" );

/////////////////////////////////////////////////////////////////////////////////
// The SearchManager class
class SearchManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( SearchManager::$m_hInstance == NULL )
            SearchManager::$m_hInstance = new SearchManager();
        return SearchManager::$m_hInstance;
    }
    protected static $m_hInstance;
	
	// Constructor / Destructor
    public function __construct()
    {
		$this->_GenerateTables();
    }
	public function __destruct()
    {
		// nothing to do
	}
	
	// Main routines
	public function InitializeTagRequests()
	{
		$hAJAX = AJAXManager::GetSingleton();

		$hAJAX->GenerateTagRequest( "SearchContent", "PHP/Modules/Search/_AJAX_SearchContent.php", array( "SearchEvent", "SearchEventParam" ),
																								   "UpdateMenuAfterContent(false);" );
		$hAJAX->UpdateTag( false, "SearchContent", array( "'DisplayResults'", "'0'" ) );
	}
	
	public function Query( $strSearchString, $iOptions = SEARCH_OPTION_ALL )
	{
		$hSQL = SQLManager::GetSingleton();
		
		// Clear results table
		$hSQL->Delete( "_PHP_Search_Results" );
		
		// Parse search string
		$arrSearchTree = explode( "|", $strSearchString );
		$iTreeCount = count( $arrSearchTree );
		
		for( $i = 0; $i < $iTreeCount; ++$i ) {
			$arrSearchTree[$i] = explode( "&", $arrSearchTree[$i] );
			$iCount = count( $arrSearchTree[$i] );
			for( $j = 0; $j < $iCount; ++$j )
				$arrSearchTree[$i][$j] = trim( $arrSearchTree[$i][$j] );
		}

		// Build SQL conditions and parameters
		$strConditions = "";
		$arrConditionValues = array();
		for( $i = 0; $i < $iTreeCount; ++$i ) {
			$iCount = count( $arrSearchTree[$i] );
			for( $j = 0; $j < $iCount; ++$j ) {
				$strConditions .= "Content LIKE '?'";
				array_push( $arrConditionValues, "LARG" );
				array_push( $arrConditionValues, $arrSearchTree[$i][$j] );
				if ( $j < ($iCount - 1) )
					$strConditions .= " AND ";
			}
			if ( $i < ($iTreeCount - 1) )
				$strConditions .= " OR ";
		}

		// Search in Forum
			// Search Messages
		if ( $iOptions & SEARCH_OPTION_MESSAGES ) {
			$hSQL->Select( "_PHP_Forum_Messages", NULL,
						   $strConditions, $arrConditionValues, "ThreadID" );

			$arrResults = array();
			$arrResultRow = $hSQL->FetchRow();
			while( $arrResultRow != NULL ) {
				// Rank result, this time account for matched parts
				// of $arrSearchTree and give weights based on OR/AND depth
				// and number of occurences ...
				$iRank = $this->_ComputeRank( $arrResultRow["Content"], $arrSearchTree, $iTreeCount );
				
				// Add to results table
				

				$arrResultRow = $hSQL->FetchRow();
			}
			$hSQL->FreeResults();
		}
		// Search in CodeDoc
			// Search Directories
		if ( $iOptions & SEARCH_OPTION_DIRECTORIES ) {
		}
			// Search Files
		if ( $iOptions & SEARCH_OPTION_FILES ) {
		}
			// Search Defines
		if ( $iOptions & SEARCH_OPTION_DEFINES ) {
		}
			// Search Macros
		if ( $iOptions & SEARCH_OPTION_MACROS ) {
		}
			// Search Namespaces
		if ( $iOptions & SEARCH_OPTION_NAMESPACES ) {
		}
			// Search Types
		if ( $iOptions & SEARCH_OPTION_TYPES ) {
		}
			// Search Enum
		if ( $iOptions & SEARCH_OPTION_ENUMS ) {
		}
			// Search Typedefs
		if ( $iOptions & SEARCH_OPTION_TYPEDEFS ) {
		}
			// Search Variables
		if ( $iOptions & SEARCH_OPTION_VARIABLES ) {
		}
			// Search Functions
		if ( $iOptions & SEARCH_OPTION_FUNCTIONS ) {
		}
	}

	public function Display()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "SearchContent" );
		$hHTML->DivStart();
			$this->_DisplayContent( SEARCH_DISPLAY_RESULTS, "0" );
		$hHTML->DivEnd();
	}
	
	// Internal routines (don't call yourself !)
	public function _DisplayContent( $strEvent, $strEventParam )
	{
		// Update from event
		$this->_Update( $strEvent, $strEventParam );

		// Display response
		switch( $strEvent ) {
			case SEARCH_DISPLAY_RESULTS:
				$iPageIndex = intval( $strEventParam );
				$this->_DisplaySearchResults( $iPageIndex );
				break;
			default: assert( 'false' ); break;
		}
	}
	
	// Helpers
	private function _GenerateTables()
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->ConnectTemporary();

		// Sections table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Search_Results ( ";
		$strQuery .= "ResultID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "Ranking int NOT NULL, ";
		$strQuery .= "Source char(8) NOT NULL, "; // Forum/CodeDoc
		$strQuery .= "Kind char(16) NOT NULL, "; // Message/Directory/File/Define/Macro/Namespace/Type/Enum/Typedef/Variable/Function
		$strQuery .= "TargetID int UNSIGNED NOT NULL, ";
		$strQuery .= "Label char(64) NOT NULL, ";
		$strQuery .= "Extract tinytext NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( ResultID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );
		
		$hSQL->DisconnectTemporary();
	}

		// Update routines
	private function _Update( & $strEvent, & $strEventParam )
	{
		switch( $strEvent ) {
//			case SEARCH_EVENT_:
//				$arrParameters = explode( "##", $strEventParam );
//				$i = intval( $arrParameters[0] );
//				$str = $arrParameters[1];
//
//				$strEvent = SEARCH_DISPLAY_RESULTS;
//				$strEventParam = "1";
//				break;
			default:
				// nothing to do
				break;
		}
	}

		// Display routines
	private function _DisplaySearchResults( $iPageIndex )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$hHTML->SetID( "SearchContent_Header" );
		$hHTML->SetStyleClass( "ClearFix" );
		$hHTML->DivStart();
		$hHTML->DivEnd();

		$hHTML->SetID( "SearchContent_Results" );
		$hHTML->DivStart();
		$hHTML->DivEnd();

		$hHTML->SetID( "SearchContent_Footer" );
		$hHTML->SetStyleClass( "ClearFix" );
		$hHTML->DivStart();
		$hHTML->DivEnd();
	}

    // Members
}
?>