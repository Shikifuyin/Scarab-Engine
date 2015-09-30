<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Forum/Forum.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Modules, Forum
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

	// Default section, allways exists
define( "FORUM_SECTION_DEFAULT", 0 );

	// Page sizes
define( "FORUM_THREADS_BY_PAGE",  3 );
define( "FORUM_MESSAGES_BY_PAGE", 3 );

	// Forum events
define( "FORUM_DISPLAY_ROOT",         "DisplayRoot" );
define( "FORUM_DISPLAY_BOARD",        "DisplayBoard" );
define( "FORUM_DISPLAY_THREAD",       "DisplayThread" );
define( "FORUM_DISPLAY_THREAD_NEW",   "DisplayThreadNew" );
define( "FORUM_DISPLAY_MESSAGE_NEW",  "DisplayMessageNew" );
define( "FORUM_DISPLAY_MESSAGE_EDIT", "DisplayMessageEdit" );
define( "FORUM_DISPLAY_ADMIN",        "DisplayAdmin" );

define( "FORUM_EVENT_THREAD_NEW",           "ThreadNew" );
define( "FORUM_EVENT_MESSAGE_NEW",          "MessageNew" );
define( "FORUM_EVENT_MESSAGE_EDIT",         "MessageEdit" );
define( "FORUM_EVENT_ADMIN_SECTION_APPLY",  "AdminSectionApply" );
define( "FORUM_EVENT_ADMIN_SECTION_DELETE", "AdminSectionDelete" );
define( "FORUM_EVENT_ADMIN_BOARD_APPLY",    "AdminBoardApply" );
define( "FORUM_EVENT_ADMIN_BOARD_DELETE",   "AdminBoardDelete" );
define( "FORUM_EVENT_ADMIN_BOARD_MOVE",     "AdminBoardMove" );
define( "FORUM_EVENT_ADMIN_THREAD_APPLY",   "AdminThreadApply" );
define( "FORUM_EVENT_ADMIN_THREAD_DELETE",  "AdminThreadDelete" );
define( "FORUM_EVENT_ADMIN_THREAD_MOVE",    "AdminThreadMove" );
define( "FORUM_EVENT_ADMIN_MESSAGE_APPLY",  "AdminMessageApply" );
define( "FORUM_EVENT_ADMIN_MESSAGE_DELETE", "AdminMessageDelete" );
define( "FORUM_EVENT_ADMIN_MESSAGE_MOVE",   "AdminMessageMove" );

/////////////////////////////////////////////////////////////////////////////////
// The ForumManager class
class ForumManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( ForumManager::$m_hInstance == NULL )
            ForumManager::$m_hInstance = new ForumManager();
        return ForumManager::$m_hInstance;
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

	// Sections operations
	public function CreateSection( $iShowIndex, $strName, $bPublic )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Insert( "_PHP_Forum_Sections", array( "ShowIndex" => $iShowIndex,
													 "Name"      => $strName,
													 "Public"    => $bPublic ? "1" : "0" ) );
		return $hSQL->GetAutoIncrement();
	}
	public function DestroySection( $iSectionID )
	{
		$hSQL = SQLManager::GetSingleton();

		// Move all boards to default section
		$hSQL->Update( "_PHP_Forum_Boards", array( "SectionID" => 0 ),
					   "SectionID=?", array( $iSectionID ) );

		// Delete the section
		$hSQL->Delete( "_PHP_Forum_Sections",
					   "SectionID=?", array( $iSectionID ) );
	}
	public function UpdateSection( $iSectionID, $iShowIndex, $strName, $bPublic )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Update( "_PHP_Forum_Sections", array( "ShowIndex" => $iShowIndex,
													 "Name" => $strName,
													 "Public" => $bPublic ? "1" : "0" ),
					   "SectionID=?", array( $iSectionID ) );
	}
	public function ListSections( $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "SectionID" );
		$hSQL->Select( "_PHP_Forum_Sections", $arrKeys, NULL, NULL, "ShowIndex" );

		$arrSections = array();
		$arrSectionRow = $hSQL->FetchRow();
		while( $arrSectionRow != NULL ) {
			if ( $bGetInfos ) {
				$arrSectionRow["Public"] = ( $arrSectionRow["Public"] == "1" );
				array_push( $arrSections, $arrSectionRow );
			} else
				array_push( $arrSections, $arrSectionRow["SectionID"] );
			$arrSectionRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrSections;
	}
	public function GetSectionInfos( $iSectionID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_Forum_Sections", NULL,
					   "SectionID=?", array( $iSectionID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrSectionRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrSectionRow["Public"] = ( $arrSectionRow["Public"] == "1" );
		return $arrSectionRow;
	}

	// Boards operations
	public function CreateBoard( $iSectionID, $iShowIndex, $strName, $strDescription )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Insert( "_PHP_Forum_Boards", array( "SectionID"   => $iSectionID,
												   "ShowIndex"   => $iShowIndex,
												   "StickyCount" => 0,
												   "ThreadCount" => 0,
												   "Name"        => $strName,
												   "Description" => $strDescription ) );
		return $hSQL->GetAutoIncrement();
	}
	public function DestroyBoard( $iBoardID )
	{
		$hSQL = SQLManager::GetSingleton();

		// Delete all messages from threads in the board
		$strQuery = "DELETE FROM _PHP_Forum_Messages WHERE ThreadID IN";
		$strQuery .= " ( SELECT ThreadID FROM _PHP_Forum_Threads WHERE BoardID=? )";

		$bOK = $hSQL->Query( $strQuery, "$iBoardID" );
		assert( '$bOK' );

		// Delete all threads in the board
		$hSQL->Delete( "_PHP_Forum_Threads",
					   "BoardID=?", array( $iBoardID ) );

		// Delete the board
		$hSQL->Delete( "_PHP_Forum_Boards",
					   "BoardID=?", array( $iBoardID ) );
	}
	public function MoveBoard( $iBoardID, $iSectionID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_Forum_Boards", array( "SectionID" => $iSectionID ),
					   "BoardID=?", array( $iBoardID ) );
	}
	public function UpdateBoard( $iBoardID, $iShowIndex, $strName, $strDescription )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_Forum_Boards", array( "ShowIndex"   => $iShowIndex,
												   "Name"        => $strName,
												   "Description" => $strDescription ),
					   "BoardID=?", array( $iBoardID ) );
	}
	public function ListBoards( $iSectionID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "BoardID" );
		$hSQL->Select( "_PHP_Forum_Boards", $arrKeys,
					   "SectionID=?", array( $iSectionID ),
					   "ShowIndex" );

		$arrBoards = array();
		$arrBoardRow = $hSQL->FetchRow();
		while( $arrBoardRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrBoards, $arrBoardRow );
			else
				array_push( $arrBoards, $arrBoardRow["BoardID"] );
			$arrBoardRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrBoards;
	}
	public function GetBoardInfos( $iBoardID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_Forum_Boards", NULL,
					   "BoardID=?", array( $iBoardID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrBoardRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrBoardRow;
	}

	// Threads operations
	public function CreateThread( $iBoardID, $iUserID, $strSubject, $bSticky = false, $bClosed = false, $bArchive = false )
	{
		$hSQL = SQLManager::GetSingleton();
		
		if ( $bSticky )
			$this->_UpdateBoardStickyCount( $iBoardID, +1 );
		else
			$this->_UpdateBoardThreadCount( $iBoardID, +1 );
		
		$hSQL->Insert( "_PHP_Forum_Threads", array( "BoardID"      => $iBoardID,
												    "UserID"       => $iUserID,
												    "MessageCount" => 0,
												    "ViewCount"    => 0,
												    "Subject"      => $strSubject,
												    "Sticky"       => $bSticky ? "1" : "0",
												    "Closed"       => $bClosed ? "1" : "0",
												    "Archive"      => $bArchive ? "1" : "0" ) );
		return $hSQL->GetAutoIncrement();
	}
	public function DestroyThread( $iThreadID )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrThreadInfos = $this->GetThreadInfos( $iThreadID );
		$iBoardID = $arrThreadInfos["BoardID"];
		$bSticky = $arrThreadInfos["Sticky"];
		
		if ( $bSticky )
			$this->_UpdateBoardStickyCount( $iBoardID, -1 );
		else
			$this->_UpdateBoardThreadCount( $iBoardID, -1 );

		// Delete all messages in the thread
		$hSQL->Delete( "_PHP_Forum_Messages",
					   "ThreadID=?", array( $iThreadID ) );

		// Delete the thread
		$hSQL->Delete( "_PHP_Forum_Threads",
					   "ThreadID=?", array( $iThreadID ) );
	}
	public function MoveThread( $iThreadID, $iBoardID )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrThreadInfos = $this->GetThreadInfos( $iThreadID );
		$iOldBoardID = $arrThreadInfos["BoardID"];
		$bSticky = $arrThreadInfos["Sticky"];

		if ( $iOldBoardID == $iBoardID )
			return;
			
		if ( $bSticky ) {
			$this->_UpdateBoardStickyCount( $iOldBoardID, -1 );
			$this->_UpdateBoardStickyCount( $iBoardID, +1 );
		} else {
			$this->_UpdateBoardThreadCount( $iOldBoardID, -1 );
			$this->_UpdateBoardThreadCount( $iBoardID, +1 );
		}

		$hSQL->Update( "_PHP_Forum_Threads", array( "BoardID" => $iBoardID ),
					   "ThreadID=?", array( $iThreadID ) );
	}
	public function UpdateThread( $iThreadID, $strSubject, $bSticky, $bClosed, $bArchive )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrThreadInfos = $this->GetThreadInfos( $iThreadID );
		$iBoardID = $arrThreadInfos["BoardID"];
		$bWasSticky = $arrThreadInfos["Sticky"];

		if ( $bSticky != $bWasSticky ) {
			if ( $bSticky ) {
				$this->_UpdateBoardStickyCount( $iBoardID, +1 );
				$this->_UpdateBoardThreadCount( $iBoardID, -1 );
			} else {
				$this->_UpdateBoardStickyCount( $iBoardID, -1 );
				$this->_UpdateBoardThreadCount( $iBoardID, +1 );
			}
		}
		
		$hSQL->Update( "_PHP_Forum_Threads", array( "Subject" => $strSubject,
												    "Sticky"  => $bSticky ? "1" : "0",
													"Closed"  => $bClosed ? "1" : "0",
												    "Archive" => $bArchive ? "1" : "0" ),
					   "ThreadID=?", array( $iThreadID ) );
	}
	public function ListThreads( $iBoardID, $bSticky, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "ThreadID" );
		$hSQL->Select( "_PHP_Forum_Threads", $arrKeys,
					   "BoardID=? AND Sticky=?", array( $iBoardID, $bSticky ? "1" : "0" ),
					   "LastAccess DESC" );

		$arrThreads = array();
		$arrThreadRow = $hSQL->FetchRow();
		while( $arrThreadRow != NULL ) {
			if ( $bGetInfos ) {
				$arrThreadRow["Sticky"] = ( $arrThreadRow["Sticky"] == "1" );
				$arrThreadRow["Closed"] = ( $arrThreadRow["Closed"] == "1" );
				$arrThreadRow["Archive"] = ( $arrThreadRow["Archive"] == "1" );
				array_push( $arrThreads, $arrThreadRow );
			} else
				array_push( $arrThreads, $arrThreadRow["ThreadID"] );
			$arrThreadRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrThreads;
	}
	public function GetThreadInfos( $iThreadID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_Forum_Threads", NULL,
					   "ThreadID=?", array( $iThreadID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrThreadRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrThreadRow["Sticky"] = ( $arrThreadRow["Sticky"] == "1" );
		$arrThreadRow["Closed"] = ( $arrThreadRow["Closed"] == "1" );
		$arrThreadRow["Archive"] = ( $arrThreadRow["Archive"] == "1" );
		return $arrThreadRow;
	}

	// Messages operations
	public function CreateMessage( $iThreadID, $iUserID, $strContent, $bStarter = false )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strDateTime = date( "Y-m-d H:i:s" );

		$this->_UpdateThreadMessageCount( $iThreadID, +1 );
		
		$hSQL->Insert( "_PHP_Forum_Messages", array( "ThreadID"     => $iThreadID,
												     "UserID"       => $iUserID,
												     "Content"      => $strContent,
												     "CreationTime" => $strDateTime,
												     "Starter"      => $bStarter ? "1" : "0" ) );
		return $hSQL->GetAutoIncrement();
	}
	public function DestroyMessage( $iMessageID )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrMessageInfos = $this->GetMessageInfos( $iMessageID );
		$iThreadID = $arrMessageInfos["ThreadID"];
		if ( $arrMessageInfos["Starter"] )
			return false; // Destroy thread instead

		$this->_UpdateThreadMessageCount( $iThreadID, -1 );

		$hSQL->Delete( "_PHP_Forum_Messages",
					   "MessageID=?", array( $iMessageID ) );

		return true;
	}
	public function MoveMessage( $iMessageID, $iThreadID )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrMessageInfos = $this->GetMessageInfos( $iMessageID );
		$iOldThreadID = $arrMessageInfos["ThreadID"];
		if ( $arrMessageInfos["Starter"] )
			return false; // Move thread instead

		if ( $iOldThreadID == $iThreadID )
			return true;

		$this->_UpdateThreadMessageCount( $iOldThreadID, -1 );
		$this->_UpdateThreadMessageCount( $iThreadID, +1 );
		
		$hSQL->Update( "_PHP_Forum_Messages", array( "ThreadID" => $iThreadID ),
					   "MessageID=?", array( $iMessageID ) );

		return true;
	}
	public function UpdateMessage( $iMessageID, $strContent )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_Forum_Messages", array( "Content" => $strContent ),
					   "MessageID=?", array( $iMessageID ) );
	}
	public function ListMessages( $iThreadID, $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();

		$arrKeys = $bGetInfos ? NULL : array( "MessageID" );
		$hSQL->Select( "_PHP_Forum_Messages", $arrKeys,
					   "ThreadID=?", array( $iThreadID ),
					   "CreationTime" );

		$arrMessages = array();
		$arrMessageRow = $hSQL->FetchRow();
		while( $arrMessageRow != NULL ) {
			if ( $bGetInfos ) {
				$arrMessageRow["Starter"] = ( $arrMessageRow["Starter"] == "1" );
				array_push( $arrMessages, $arrMessageRow );
			} else
				array_push( $arrMessages, $arrMessageRow["MessageID"] );
			$arrMessageRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrMessages;
	}
	public function GetMessageInfos( $iMessageID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Select( "_PHP_Forum_Messages", NULL,
					   "MessageID=?", array( $iMessageID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrMessageRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$arrMessageRow["Starter"] = ( $arrMessageRow["Starter"] == "1" );
		return $arrMessageRow;
	}

	// Main routines
	public function InitializeTagRequests()
	{
		$hAJAX = AJAXManager::GetSingleton();

		$hAJAX->GenerateTagRequest( "ForumContent", "PHP/Modules/Forum/_AJAX_ForumContent.php", array( "ForumEvent", "ForumEventParam" ),
																								"UpdateMenuAfterContent(false);" );
		$hAJAX->GenerateTagRequest( "ForumHeader", "PHP/Modules/Forum/_AJAX_ForumHeader.php", array( "ForumEvent", "ForumEventParam" ),
																							  "UpdateMenuAfterContent(false);" );
		$hAJAX->UpdateTag( false, "ForumContent", array( "'DisplayRoot'", "''" ) );
		$hAJAX->UpdateTag( false, "ForumHeader", array( "'DisplayRoot'", "''" ) );
	}

	public function Display()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "ForumHeader" );
		$hHTML->SetStyleClass( "ForumContainer_Level0 ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayHeader( FORUM_DISPLAY_ROOT, "" );
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent" );
		$hHTML->SetStyleClass( "ForumContainer_Level0 ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayContent( FORUM_DISPLAY_ROOT, "" );
		$hHTML->DivEnd();
	}

	// Internal routines (don't call yourself !)
	public function _DisplayHeader( $strEvent, $strEventParam )
	{
		$hHTML = HTMLManager::GetSingleton();

		$hHTML->SetID( "ForumHeader_PathLink" );
		$hHTML->SetStyleClass( "ForumContainer_Level1H ForumProperties_H_LeftContainer ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayPathLink( $strEvent, $strEventParam );
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumHeader_Commands" );
		$hHTML->SetStyleClass( "ForumContainer_Level1H ForumProperties_H_RightContainer ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayNavigationBox( $strEvent, $strEventParam );
			$this->_DisplayAdminButton();
		$hHTML->DivEnd();
	}
	public function _DisplayContent( $strEvent, $strEventParam )
	{
		// Update from event
		$this->_Update( $strEvent, $strEventParam );

		// Display response
		switch( $strEvent ) {
			case FORUM_DISPLAY_ROOT:
				$this->_DisplayRoot();
				break;
			case FORUM_DISPLAY_BOARD:
				$arrParameters = explode( "##", $strEventParam );
				$iBoardID = intval( $arrParameters[0] );
				$iPageIndex = intval( $arrParameters[1] );
				$this->_DisplayBoard( $iBoardID, $iPageIndex );
				break;
			case FORUM_DISPLAY_THREAD:
				$arrParameters = explode( "##", $strEventParam );
				$iThreadID = intval( $arrParameters[0] );
				$iPageIndex = intval( $arrParameters[1] );
				if ( $arrParameters[1] == "V" ) {
					$this->_UpdateThreadViewCount( $iThreadID, +1 );
					$iPageIndex = 0;
				}
				$this->_DisplayThread( $iThreadID, $iPageIndex );
				break;
			case FORUM_DISPLAY_THREAD_NEW:
				$arrParameters = explode( "##", $strEventParam );
				$iBoardID = intval( $arrParameters[0] );
				$iPageIndex = intval( $arrParameters[1] );
				$this->_DisplayThreadNew( $iBoardID, $iPageIndex );
				break;
			case FORUM_DISPLAY_MESSAGE_NEW:
				$arrParameters = explode( "##", $strEventParam );
				$iThreadID = intval( $arrParameters[0] );
				$iPageIndex = intval( $arrParameters[1] );
				$this->_DisplayMessageNew( $iThreadID, $iPageIndex );
				break;
			case FORUM_DISPLAY_MESSAGE_EDIT:
				$arrParameters = explode( "##", $strEventParam );
				$iMessageID = intval( $arrParameters[0] );
				$iPageIndex = intval( $arrParameters[1] );
				$this->_DisplayMessageEdit( $iMessageID, $iPageIndex );
				break;
			case FORUM_DISPLAY_ADMIN:
				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iThreadID = intval( $arrParameters[2] );
				$iMessageID = intval( $arrParameters[3] );
				$this->_DisplayAdmin( $iSectionID, $iBoardID, $iThreadID, $iMessageID );
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
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Forum_Sections ( ";
		$strQuery .= "SectionID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ShowIndex int UNSIGNED NOT NULL, ";
		$strQuery .= "Name char(64) NOT NULL, ";
		$strQuery .= "Public tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( SectionID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Boards table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Forum_Boards ( ";
		$strQuery .= "BoardID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "SectionID int UNSIGNED NOT NULL, ";
		$strQuery .= "ShowIndex int UNSIGNED NOT NULL, ";
		$strQuery .= "StickyCount int UNSIGNED NOT NULL, ";
		$strQuery .= "ThreadCount int UNSIGNED NOT NULL, ";
		$strQuery .= "Name char(64) NOT NULL, ";
		$strQuery .= "Description tinytext NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( BoardID ), ";
		$strQuery .= "FOREIGN KEY ( SectionID ) REFERENCES _PHP_Forum_Sections( SectionID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Threads table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Forum_Threads ( ";
		$strQuery .= "ThreadID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "BoardID int UNSIGNED NOT NULL, ";
		$strQuery .= "UserID int UNSIGNED NOT NULL, ";
		$strQuery .= "MessageCount int UNSIGNED NOT NULL, ";
		$strQuery .= "ViewCount int UNSIGNED NOT NULL, ";
		$strQuery .= "LastAccess timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP, ";
		$strQuery .= "Subject char(64) NOT NULL, ";
		$strQuery .= "Sticky tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Closed tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Archive tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( ThreadID ), ";
		$strQuery .= "FOREIGN KEY ( BoardID ) REFERENCES _PHP_Forum_Boards( BoardID ), ";
		$strQuery .= "FOREIGN KEY ( UserID ) REFERENCES _PHP_Users( UserID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		// Messages table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Forum_Messages ( ";
		$strQuery .= "MessageID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "ThreadID int UNSIGNED NOT NULL, ";
		$strQuery .= "UserID int UNSIGNED NOT NULL, ";
		$strQuery .= "LastAccess timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP, ";
		$strQuery .= "Content text NOT NULL, ";
		$strQuery .= "CreationTime datetime NOT NULL, ";
		$strQuery .= "Starter tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( MessageID ), ";
		$strQuery .= "FOREIGN KEY ( ThreadID ) REFERENCES _PHP_Forum_Threads( ThreadID ), ";
		$strQuery .= "FOREIGN KEY ( UserID ) REFERENCES _PHP_Users( UserID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );

		$hSQL->DisconnectTemporary();
	}

		// Update routines
	private function _UpdateBoardStickyCount( $iBoardID, $iIncrease )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strQuery = "UPDATE _PHP_Forum_Boards";
		if ( $iIncrease == +1 )
			$strQuery .= " SET StickyCount=StickyCount+1";
		else {
			assert( '$iIncrease == -1' );
			$strQuery .= " SET StickyCount=StickyCount-1";
		}
		$strQuery .= " WHERE BoardID=?";

		$bOK = $hSQL->Query( $strQuery, "$iBoardID" );
		assert( '$bOK' );
	}
	private function _UpdateBoardThreadCount( $iBoardID, $iIncrease )
	{
		$hSQL = SQLManager::GetSingleton();

		$strQuery = "UPDATE _PHP_Forum_Boards";
		if ( $iIncrease == +1 )
			$strQuery .= " SET ThreadCount=ThreadCount+1";
		else {
			assert( '$iIncrease == -1' );
			$strQuery .= " SET ThreadCount=ThreadCount-1";
		}
		$strQuery .= " WHERE BoardID=?";

		$bOK = $hSQL->Query( $strQuery, "$iBoardID" );
		assert( '$bOK' );
	}
	private function _ComputeBoardMessageCount( $iBoardID )
	{
		$hSQL = SQLManager::GetSingleton();

		$strQuery = "SELECT SUM(MessageCount) FROM _PHP_Forum_Threads WHERE BoardID=?";

		$bOK = $hSQL->Query( $strQuery, "$iBoardID" );
		assert( '$bOK' );
		
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return 0;
		}
		assert( '$hSQL->GetRowCount() == 1' );

		$arrSumRow = $hSQL->FetchRow( SQLQUERY_FETCH_ROW );
		$hSQL->FreeResults();

		if ( $arrSumRow[0] == "" )
			return 0;
		return $arrSumRow[0];
	}
	private function _ComputeBoardLastAccess( $iBoardID )
	{
		$hSQL = SQLManager::GetSingleton();

		$strQuery = "SELECT MAX(LastAccess) FROM _PHP_Forum_Threads WHERE BoardID=?";

		$bOK = $hSQL->Query( $strQuery, "$iBoardID" );
		assert( '$bOK' );
		
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );

		$arrMaxRow = $hSQL->FetchRow( SQLQUERY_FETCH_ROW );
		$hSQL->FreeResults();

		if ( $arrMaxRow[0] == "" )
			return "*Empty*";
		return $arrMaxRow[0];
	}

	private function _UpdateThreadMessageCount( $iThreadID, $iIncrease )
	{
		$hSQL = SQLManager::GetSingleton();

		$strQuery = "UPDATE _PHP_Forum_Threads";
		if ( $iIncrease == +1 )
			$strQuery .= " SET MessageCount=MessageCount+1";
		else {
			assert( '$iIncrease == -1' );
			$strQuery .= " SET MessageCount=MessageCount-1";
		}
		$strQuery .= " WHERE ThreadID=?";

		$bOK = $hSQL->Query( $strQuery, "$iThreadID" );
		assert( '$bOK' );
	}
	private function _UpdateThreadViewCount( $iThreadID, $iIncrease )
	{
		$hSQL = SQLManager::GetSingleton();

		// We need to prevent timestamp update in this case
		$strQuery = "SELECT LastAccess FROM _PHP_Forum_Threads WHERE ThreadID=?";

		$bOK = $hSQL->Query( $strQuery, "$iThreadID" );
		assert( '$bOK' );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );

		$arrThreadRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$strLastAccess = $arrThreadRow["LastAccess"];

		// Update ViewCount, override LastAccess
		$strQuery = "UPDATE _PHP_Forum_Threads";
		if ( $iIncrease == +1 )
			$strQuery .= " SET ViewCount=ViewCount+1, LastAccess='?'";
		else {
			assert( '$iIncrease == -1' );
			$strQuery .= " SET ViewCount=ViewCount-1, LastAccess='?'";
		}
		$strQuery .= " WHERE ThreadID=?";

		$bOK = $hSQL->Query( $strQuery, $strLastAccess, "$iThreadID" );
		assert( '$bOK' );
	}
	private function _UpdateThreadForceLastAccess( $iThreadID )
	{
		$hSQL = SQLManager::GetSingleton();

		$strQuery = "UPDATE _PHP_Forum_Threads SET LastAccess=CURRENT_TIMESTAMP WHERE ThreadID=?";

		$bOK = $hSQL->Query( $strQuery, "$iThreadID" );
		assert( '$bOK' );
	}

	private function _Update( & $strEvent, & $strEventParam )
	{
		$hSession = SessionManager::GetSingleton();
		$hUser = UserManager::GetSingleton();

		$bValidSession = $hSession->Open();

		switch( $strEvent ) {
			case FORUM_EVENT_THREAD_NEW:
				assert( '$bValidSession' );
				$iUserID = $hSession->GetUserID();

				$arrParameters = explode( "##", $strEventParam );
				$iBoardID = intval( $arrParameters[0] );
				$strThreadSubject = $arrParameters[1];
				$strMessageContent = $arrParameters[2];

				$iThreadID = $this->CreateThread( $iBoardID, $iUserID, $strThreadSubject, false, false, false );
				assert( '$iThreadID != NULL' );
				$iMessageID = $this->CreateMessage( $iThreadID, $iUserID, $strMessageContent, true );
				assert( '$iMessageID != NULL' );
				
				$hUser->UpdateMessageCount( $iUserID, 1 );

				$strEvent = FORUM_DISPLAY_BOARD;
				$strEventParam = $iBoardID."##"."0";
				break;
			case FORUM_EVENT_MESSAGE_NEW:
				assert( '$bValidSession' );
				$iUserID = $hSession->GetUserID();

				$arrParameters = explode( "##", $strEventParam );
				$iThreadID = intval( $arrParameters[0] );
				$strMessageContent = $arrParameters[1];

				$iMessageID = $this->CreateMessage( $iThreadID, $iUserID, $strMessageContent, false );
				assert( '$iMessageID != NULL' );
				
				$hUser->UpdateMessageCount( $iUserID, 1 );

				$arrThreadInfos = $this->GetThreadInfos( $iThreadID );
				$iMessageCount = $arrThreadInfos["MessageCount"];
				$iPageCount = floor( $iMessageCount / FORUM_MESSAGES_BY_PAGE );
				if ( $iMessageCount > ($iPageCount * FORUM_MESSAGES_BY_PAGE) )
					++$iPageCount;
				$iPageIndex = ( $iPageCount - 1 );

				$strEvent = FORUM_DISPLAY_THREAD;
				$strEventParam = $iThreadID."##"."$iPageIndex";
				break;
			case FORUM_EVENT_MESSAGE_EDIT:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iThreadID = intval( $arrParameters[0] );
				$iPageIndex = intval( $arrParameters[1] );
				$iMessageID = intval( $arrParameters[2] );
				$strMessageContent = $arrParameters[3];

				$this->UpdateMessage( $iMessageID, $strMessageContent );
				$this->_UpdateThreadForceLastAccess( $iThreadID );

				$strEvent = FORUM_DISPLAY_THREAD;
				$strEventParam = $iThreadID."##"."$iPageIndex";
				break;
			case FORUM_EVENT_ADMIN_SECTION_APPLY:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iShowIndex = intval( $arrParameters[1] );
				$strName = $arrParameters[2];
				$bPublic = ($arrParameters[3] == "1") ? true : false;

				if ( $iSectionID != NULL )
					$this->UpdateSection( $iSectionID, $iShowIndex, $strName, $bPublic );
				else {
					$iSectionID = $this->CreateSection( $iShowIndex, $strName, $bPublic );
					assert( '$iSectionID != NULL' );
				}

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##0##0##0";
				break;
			case FORUM_EVENT_ADMIN_SECTION_DELETE:
				assert( '$bValidSession' );

				$iSectionID = intval( $strEventParam );

				if ( $iSectionID != NULL )
					$this->DestroySection( $iSectionID );

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "0##0##0##0";
				break;
			case FORUM_EVENT_ADMIN_BOARD_APPLY:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iShowIndex = intval( $arrParameters[2] );
				$strName = $arrParameters[3];
				$strDescription = $arrParameters[4];

				if ( $iBoardID != NULL )
					$this->UpdateBoard( $iBoardID, $iShowIndex, $strName, $strDescription );
				else {
					$iBoardID = $this->CreateBoard( $iSectionID, $iShowIndex, $strName, $strDescription );
					assert( '$iBoardID != NULL' );
				}

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##$iBoardID##0##0";
				break;
			case FORUM_EVENT_ADMIN_BOARD_DELETE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );

				if ( $iBoardID != NULL )
					$this->DestroyBoard( $iBoardID );

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##0##0##0";
				break;
			case FORUM_EVENT_ADMIN_BOARD_MOVE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iBoardID = intval( $arrParameters[0] );
				$iSectionID = intval( $arrParameters[1] );

				if ( $iBoardID != NULL )
					$this->MoveBoard( $iBoardID, $iSectionID );

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##$iBoardID##0##0";
				break;
			case FORUM_EVENT_ADMIN_THREAD_APPLY:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iThreadID = intval( $arrParameters[2] );
				$strSubject = $arrParameters[3];
				$bSticky = ($arrParameters[4] == "1") ? true : false;
				$bClosed = ($arrParameters[5] == "1") ? true : false;
				$bArchive = ($arrParameters[6] == "1") ? true : false;

				if ( $iThreadID != NULL )
					$this->UpdateThread( $iThreadID, $strSubject, $bSticky, $bClosed, $bArchive );
				else {
					$iThreadID = $this->CreateThread( $iBoardID, $hSession->GetUserID(), $strSubject, $bSticky, $bClosed, $bArchive );
					assert( '$iThreadID != NULL' );
				}

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##$iBoardID##$iThreadID##0";
				break;
			case FORUM_EVENT_ADMIN_THREAD_DELETE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iThreadID = intval( $arrParameters[2] );

				if ( $iThreadID != NULL )
					$this->DestroyThread( $iThreadID );

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##$iBoardID##0##0";
				break;
			case FORUM_EVENT_ADMIN_THREAD_MOVE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iThreadID = intval( $arrParameters[1] );
				$iBoardID = intval( $arrParameters[2] );

				if ( $iThreadID != NULL )
					$this->MoveThread( $iThreadID, $iBoardID );

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##$iBoardID##$iThreadID##0";
				break;
			case FORUM_EVENT_ADMIN_MESSAGE_APPLY:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iThreadID = intval( $arrParameters[2] );
				$iMessageID = intval( $arrParameters[3] );
				$strContent = $arrParameters[4];
				$bStarter = ($arrParameters[5] == "1") ? true : false;

				if ( $iMessageID != NULL )
					$this->UpdateMessage( $iMessageID, $strContent );
				else {
					$iMessageID = $this->CreateMessage( $iThreadID, $hSession->GetUserID(), $strContent, $bStarter );
					assert( '$iMessageID != NULL' );
				}

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = "$iSectionID##$iBoardID##$iThreadID##$iMessageID";
				break;
			case FORUM_EVENT_ADMIN_MESSAGE_DELETE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iThreadID = intval( $arrParameters[2] );
				$iMessageID = intval( $arrParameters[3] );

				$bDestroyed = false;
				if ( $iMessageID != NULL )
					$bDestroyed = $this->DestroyMessage( $iMessageID );

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = $bDestroyed ? "$iSectionID##$iBoardID##$iThreadID##0"
											 : "$iSectionID##$iBoardID##$iThreadID##$iMessageID";
				break;
			case FORUM_EVENT_ADMIN_MESSAGE_MOVE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$iSectionID = intval( $arrParameters[0] );
				$iBoardID = intval( $arrParameters[1] );
				$iOldThreadID = intval( $arrParameters[2] );
				$iMessageID = intval( $arrParameters[3] );
				$iThreadID = intval( $arrParameters[4] );

				$bMoved = false;
				if ( $iMessageID != NULL )
					$bMoved = $this->MoveMessage( $iMessageID, $iThreadID );	

				$strEvent = FORUM_DISPLAY_ADMIN;
				$strEventParam = $bMoved ? "$iSectionID##$iBoardID##$iThreadID##$iMessageID"
										 : "$iSectionID##$iBoardID##$iOldThreadID##$iMessageID";
				break;
			default:
				// nothing to do
				break;
		}

		if ( $bValidSession )
			$hSession->Close();
	}

		// Display routines : ForumHeader
	private function _DisplayPathLink( $strEvent, $strEventParam )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetStyleClass( "ForumContainer_Level2H ForumProperties_H_LeftItem" );
		$hHTML->DivStart();
			$hAJAX->SetEventTags (
				HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
				array( array( "'DisplayRoot'", "''" ), array( "'DisplayRoot'", "''" ) )
			);
			$hHTML->DivStart();
				$hHTML->Text( "/ Root" );
			$hHTML->DivEnd();
		$hHTML->DivEnd();

		switch( $strEvent ) {
			case FORUM_DISPLAY_ROOT:
				break;
			case FORUM_DISPLAY_BOARD:
				$iBoardID = intval( $strEventParam );
				$arrBoardInfos = $this->GetBoardInfos( $iBoardID );
				$strBoardName = $arrBoardInfos["Name"];

				$hHTML->SetStyleClass( "ForumContainer_Level2H ForumProperties_H_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##0'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->DivStart();
						$hHTML->Text( "/ ". $strBoardName );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				break;
			case FORUM_DISPLAY_THREAD:
				$iThreadID = intval( $strEventParam );
				$arrThreadInfos = $this->GetThreadInfos( $iThreadID );
				$strThreadSubject = $arrThreadInfos["Subject"];

				$iBoardID = $arrThreadInfos["BoardID"];
				$arrBoardInfos = $this->GetBoardInfos( $iBoardID );
				$strBoardName = $arrBoardInfos["Name"];

				$hHTML->SetStyleClass( "ForumContainer_Level2H ForumProperties_H_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##0'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->DivStart();
						$hHTML->Text( "/ ". $strBoardName );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "ForumContainer_Level2H ForumProperties_H_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayThread'", "'$iThreadID##0'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->DivStart();
						$hHTML->Text( "/ ". $strThreadSubject );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				break;
			default: assert( 'false' ); break;
		}
	}
	private function _DisplayNavigationBox( $strEvent, $strEventParam )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();

		$bValidSession = $hSession->Open();

		$iSelectedBoardID = NULL;
		if ( $strEvent == FORUM_DISPLAY_BOARD )
			$iSelectedBoardID = intval( $strEventParam );
		else if ( $strEvent == FORUM_DISPLAY_THREAD ) {
			$iSelectedThreadID = intval( $strEventParam );
			$arrThreadInfos = $this->GetThreadInfos( $iSelectedThreadID );
			$iSelectedBoardID = $arrThreadInfos["BoardID"];
		}

		$arrSections = $this->ListSections( true );
		$iSectionCount = count( $arrSections );

		$hHTML->SetID( "ForumHeader_Commands_NavigationBox" );
		$hHTML->SetStyleClass( "ForumContainer_Level2H ForumProperties_H_RightItem" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "SelectStyler" );
			$hHTML->DivStart();
				$hHTML->InputComboListStart();
					$bSelected = ( $iSelectedBoardID == NULL );
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayRoot'", "''" ), array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->ComboListItem( "Root", NULL, $bSelected );
	
					for( $i = 0; $i < $iSectionCount; ++$i ) {
						$arrSectionInfos = $arrSections[$i];
						$iSectionID = $arrSectionInfos["SectionID"];
						$strSectionName = $arrSectionInfos["Name"];
						if ( !$bValidSession && !($arrSectionInfos["Public"]) )
							continue;
						$hHTML->ComboListGroupStart( $strSectionName );
							$arrBoards = $this->ListBoards( $iSectionID, true );
							$iBoardCount = count( $arrBoards );
							for( $j = 0; $j < $iBoardCount; ++$j ) {
								$arrBoardInfos = $arrBoards[$j];
								$iBoardID = $arrBoardInfos["BoardID"];
								$strBoardName = $arrBoardInfos["Name"];
								$bSelected = ( $iSelectedBoardID == $iBoardID );
								$hAJAX->SetEventTags (
									HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
									array( array( "'DisplayBoard'", "'$iBoardID##0'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
								);
								$hHTML->ComboListItem( $strBoardName, NULL, $bSelected );
							}
						$hHTML->ComboListGroupEnd();
					}
				$hHTML->InputComboListEnd();
			$hHTML->DivEnd();
		$hHTML->DivEnd();
		
		if ( $bValidSession )
			$hSession->Close();
	}
	private function _DisplayAdminButton()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();

		$bValidSession = $hSession->Open();

		// Admin user only
		if ( $bValidSession && $hSession->IsAdmin() ) {
			$hHTML->SetID( "ForumHeader_Commands_AdminAccess" );
			$hHTML->SetStyleClass( "ForumContainer_Level2H ForumProperties_H_RightItem" );
			$hHTML->DivStart();
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'DisplayAdmin'", "'0##0##0##0'" ), array( "'DisplayRoot'", "''" ) )
				);
				$hHTML->InputButton( "Admin" );
			$hHTML->DivEnd();
		}
		
		if ( $bValidSession )
			$hSession->Close();
	}

		// Display routines : Navigation
	private function _DisplayRoot()
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();
		
		$bValidSession = $hSession->Open();

		$arrSections = $this->ListSections( true );
		$iCount = count( $arrSections );

		for( $i = 0; $i < $iCount; ++$i ) {
			$arrSectionInfos = $arrSections[$i];
			$iSectionID = $arrSectionInfos["SectionID"];
			$strSectionName = $arrSectionInfos["Name"];
			$strPublic = $arrSectionInfos["Public"] ? "Public" : "Private";
			if ( !$bValidSession && !($arrSectionInfos["Public"]) )
				continue;

			$hHTML->SetID( "ForumContent_Root_Section_$iSectionID" );
			$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Root_Section_$iSectionID"."_Header" );
				$hHTML->SetStyleClass( "ForumContainer_Level2C ForumProperties_C_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "[$strPublic] $strSectionName" );
				$hHTML->DivEnd();

				$hHTML->SetID( "ForumContent_Root_Section_$iSectionID"."_Boards" );
				$hHTML->SetStyleClass( "ForumContainer_Level2C ClearFix" );
				$hHTML->DivStart();
					$hHTML->TableStart( true );
						$hHTML->TableRowStart();
							$hHTML->SetStyleInline( "width:64%;" );
							$hHTML->TableHeadCellStart();
								$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
								$hHTML->DivStart();
									$hHTML->Text( "Boards" );
								$hHTML->DivEnd();
							$hHTML->TableHeadCellEnd();
							$hHTML->SetStyleInline( "width:12%;" );
							$hHTML->TableHeadCellStart();
								$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
								$hHTML->DivStart();
									$hHTML->Text( "Thread count" );
								$hHTML->DivEnd();
							$hHTML->TableHeadCellEnd();
							$hHTML->SetStyleInline( "width:12%;" );
							$hHTML->TableHeadCellStart();
								$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
								$hHTML->DivStart();
									$hHTML->Text( "Message count" );
								$hHTML->DivEnd();
							$hHTML->TableHeadCellEnd();
							$hHTML->SetStyleInline( "width:12%;" );
							$hHTML->TableHeadCellStart();
								$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
								$hHTML->DivStart();
									$hHTML->Text( "Last Activity" );
								$hHTML->DivEnd();
							$hHTML->TableHeadCellEnd();
						$hHTML->TableRowEnd();

						$arrBoards = $this->ListBoards( $iSectionID, true );
						$jCount = count( $arrBoards );
						for( $j = 0; $j < $jCount; ++$j ) {
							$arrBoardInfos = $arrBoards[$j];
							$iBoardID = $arrBoardInfos["BoardID"];
							$strBoardName = $arrBoardInfos["Name"];
							$strDescription = $arrBoardInfos["Description"];
							$iTotalThreadCount = $arrBoardInfos["StickyCount"] + $arrBoardInfos["ThreadCount"];
							$iTotalMessageCount = $this->_ComputeBoardMessageCount( $iBoardID );
							$strLastAccess = $this->_ComputeBoardLastAccess( $iBoardID );

							$hHTML->TableRowStart();
								$hHTML->TableCellStart();
									$hHTML->SetID( "ForumContent_Root_Section_Boards_Link_$iBoardID" );
									$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_LinkCell" );
									$hAJAX->SetEventTags (
										HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
										array( array( "'DisplayBoard'", "'$iBoardID##0'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
									);
									$hHTML->DivStart();
										$hHTML->Text( $strBoardName, HTML_TEXT_BOLD );
										$hHTML->LineBreak();
										$hHTML->Text( $strDescription );
									$hHTML->DivEnd();
								$hHTML->TableCellEnd();
								$hHTML->TableCellStart();
									$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
									$hHTML->DivStart();
										$hHTML->Text( "$iTotalThreadCount" );
									$hHTML->DivEnd();
								$hHTML->TableCellEnd();
								$hHTML->TableCellStart();
									$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
									$hHTML->DivStart();
										$hHTML->Text( "$iTotalMessageCount" );
									$hHTML->DivEnd();
								$hHTML->TableCellEnd();
								$hHTML->TableCellStart();
									$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
									$hHTML->DivStart();
										$hHTML->Text( $strLastAccess );
									$hHTML->DivEnd();
								$hHTML->TableCellEnd();
							$hHTML->TableRowEnd();
						}
					$hHTML->TableEnd();
				$hHTML->DivEnd();
			$hHTML->DivEnd();
		}
		
		if ( $bValidSession )
			$hSession->Close();
	}
	private function _DisplayBoard( $iBoardID, $iPageIndex )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();

		$bValidSession = $hSession->Open();

		$arrBoardInfos = $this->GetBoardInfos( $iBoardID );
		$iStickyCount = $arrBoardInfos["StickyCount"];
		$iThreadCount = $arrBoardInfos["ThreadCount"];
		$strName = $arrBoardInfos["Name"];
		$strDescription = $arrBoardInfos["Description"];
		
		// Compute page count & last page size
		$iPageCount = floor( $iThreadCount / FORUM_THREADS_BY_PAGE );
		$iLastPageSize = $iThreadCount - ( $iPageCount * FORUM_THREADS_BY_PAGE );
		$bHasLastPage = ( $iLastPageSize > 0 );
		if ( $bHasLastPage )
			++$iPageCount;

		// Compute next, prev, last & display page indices
		$iLastPageIndex = max( $iPageCount - 1, 0 );
		$iPrevPageIndex = max( $iPageIndex - 1, 0 );
		$iNextPageIndex = min( $iPageIndex + 1, $iLastPageIndex );
		$iDisplayIndex = ( $iPageCount > 0 ) ? $iPageIndex + 1 : 0;

		// Compute thread range
		$arrStickyThreads = $this->ListThreads( $iBoardID, true, true );
		$arrThreads = $this->ListThreads( $iBoardID, false, true );
		$iThreadRangeStart = ( $iPageIndex * FORUM_THREADS_BY_PAGE );
		$iThreadRangeSize = 0;
		if ( $iPageCount > 0 ) {
			$iThreadRangeSize = FORUM_THREADS_BY_PAGE;
			if ( $bHasLastPage && ($iPageIndex == $iLastPageIndex) )
				$iThreadRangeSize = $iLastPageSize;
		}

		$hHTML->SetID( "ForumContent_Board_Header" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "ForumContent_Board_Header_Description" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CH ForumProperties_CH_LeftText" );
			$hHTML->DivStart();
				$hHTML->Text( $strName, HTML_TEXT_BOLD );
				$hHTML->LineBreak();
				$hHTML->Text( $strDescription );
			$hHTML->DivEnd();

			$hHTML->SetID( "ForumContent_Board_Header_Commands" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CH ForumProperties_CH_RightContainer ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Board_Header_Commands_ParentButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CH ForumProperties_CH_RightItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayRoot'", "''" ), array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->InputButton( "Parent" );
				$hHTML->DivEnd();

				// Logged user only
				if ( $bValidSession ) {
					$hHTML->SetID( "ForumContent_Board_Header_Commands_NewThreadButton" );
					$hHTML->SetStyleClass( "ForumContainer_Level3CH ForumProperties_CH_RightItem" );
					$hHTML->DivStart();
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayThreadNew'", "'$iBoardID##$iPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
						);
						$hHTML->InputButton( "New Thread" );
					$hHTML->DivEnd();
				}
			$hHTML->DivEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_Board_StickyThreads" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->TableStart( true );
				$hHTML->TableRowStart();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "State" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:52%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "Subject" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "Message count" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "View count" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "Last Activity" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
				$hHTML->TableRowEnd();
				
				for( $i = 0; $i < $iStickyCount; ++$i ) {
					$arrThreadInfos = $arrStickyThreads[$i];
					$iThreadID = $arrThreadInfos["ThreadID"];
					$strSubject = $arrThreadInfos["Subject"];
					$iMessageCount = $arrThreadInfos["MessageCount"];
					$iViewCount = $arrThreadInfos["ViewCount"];
					$strLastAccess = $arrThreadInfos["LastAccess"];

					$strThreadState = ( $arrThreadInfos["Sticky"] ) ? "S/" : "-/";
					$strThreadState .= ( $arrThreadInfos["Closed"] ) ? "C/" : "-/";
					$strThreadState .= ( $arrThreadInfos["Archive"] ) ? "A" : "-";

					$hHTML->TableRowStart();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( $strThreadState );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetID( "ForumContent_Board_StickyThreads_Link_$iThreadID" );
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_LinkCell" );
							$hAJAX->SetEventTags (
								HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
								array( array( "'DisplayThread'", "'$iThreadID##V'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
							);
							$hHTML->DivStart();
								$hHTML->Text( $strSubject );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( "$iMessageCount" );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( "$iViewCount" );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( $strLastAccess );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
					$hHTML->TableRowEnd();
				}
			$hHTML->TableEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_Board_Threads" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->TableStart( true );
				$hHTML->TableRowStart();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "State" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:52%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "Subject" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "Message count" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "View count" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
					$hHTML->SetStyleInline( "width:12%;" );
					$hHTML->TableHeadCellStart();
						$hHTML->SetStyleClass( "ForumContainer_TableHeadCell" );
						$hHTML->DivStart();
							$hHTML->Text( "Last Activity" );
						$hHTML->DivEnd();
					$hHTML->TableHeadCellEnd();
				$hHTML->TableRowEnd();

				for( $i = 0; $i < $iThreadRangeSize; ++$i ) {
					$arrThreadInfos = $arrThreads[$iThreadRangeStart + $i];
					$iThreadID = $arrThreadInfos["ThreadID"];
					$strSubject = $arrThreadInfos["Subject"];
					$iMessageCount = $arrThreadInfos["MessageCount"];
					$iViewCount = $arrThreadInfos["ViewCount"];
					$strLastAccess = $arrThreadInfos["LastAccess"];

					$strThreadState = ( $arrThreadInfos["Sticky"] ) ? "S/" : "-/";
					$strThreadState .= ( $arrThreadInfos["Closed"] ) ? "C/" : "-/";
					$strThreadState .= ( $arrThreadInfos["Archive"] ) ? "A" : "-";

					$hHTML->TableRowStart();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( $strThreadState );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetID( "ForumContent_Board_Threads_Link_$iThreadID" );
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_LinkCell" );
							$hAJAX->SetEventTags (
								HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
								array( array( "'DisplayThread'", "'$iThreadID##V'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
							);
							$hHTML->DivStart();
								$hHTML->Text( $strSubject );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( "$iMessageCount" );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( "$iViewCount" );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
						$hHTML->TableCellStart();
							$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_TextCell" );
							$hHTML->DivStart();
								$hHTML->Text( $strLastAccess );
							$hHTML->DivEnd();
						$hHTML->TableCellEnd();
					$hHTML->TableRowEnd();
				}
			$hHTML->TableEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_Board_Footer" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "ForumContent_Board_Footer_Navigation" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_LeftContainer ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Board_Footer_Navigation_FirstPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##0'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "First" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Board_Footer_Navigation_PrevPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##$iPrevPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Prev" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Board_Footer_Navigation_PageIndex" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem ForumProperties_CF_PageIndex" );
				$hHTML->DivStart();
					$hHTML->Text( "$iDisplayIndex / $iPageCount" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Board_Footer_Navigation_NextPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##$iNextPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Next" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Board_Footer_Navigation_LastPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##$iLastPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Last" );
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetID( "ForumContent_Board_Footer_Commands" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightContainer ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Board_Footer_Commands_ParentButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_RightItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayRoot'", "''" ), array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->InputButton( "Parent" );
				$hHTML->DivEnd();

				// Logged user only
				if ( $bValidSession ) {
					$hHTML->SetID( "ForumContent_Board_Footer_Commands_NewThreadButton" );
					$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_RightItem" );
					$hHTML->DivStart();
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayThreadNew'", "'$iBoardID##$iPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
						);
						$hHTML->InputButton( "New Thread" );
					$hHTML->DivEnd();
				}
			$hHTML->DivEnd();
		$hHTML->DivEnd();
		
		if ( $bValidSession )
			$hSession->Close();
	}
	private function _DisplayThread( $iThreadID, $iPageIndex )
	{
		$hSystem = SystemManager::GetSingleton();
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();
		$hUser = UserManager::GetSingleton();

		$bValidSession = $hSession->Open();

		$arrThreadInfos = $this->GetThreadInfos( $iThreadID );
		$iBoardID = $arrThreadInfos["BoardID"];
		$iMessageCount = $arrThreadInfos["MessageCount"];
		$strSubject = $arrThreadInfos["Subject"];
		$bSticky = $arrThreadInfos["Sticky"];
		$bClosed = $arrThreadInfos["Closed"];

		// Compute index of that thread in its board
		$iBoardPageIndex = 0;
		if ( !$bSticky ) {
			/////////////////////////////////////////////////////////
			//$iBoardPageIndex = ;
		}

		// Compute page count & last page size
		$iPageCount = floor( $iMessageCount / FORUM_MESSAGES_BY_PAGE );
		$iLastPageSize = $iMessageCount - ( $iPageCount * FORUM_MESSAGES_BY_PAGE );
		$bHasLastPage = ( $iLastPageSize > 0 );
		if ( $bHasLastPage )
			++$iPageCount;

		// Compute next, prev, last & display page indices
		$iLastPageIndex = max( $iPageCount - 1, 0 );
		$iPrevPageIndex = max( $iPageIndex - 1, 0 );
		$iNextPageIndex = min( $iPageIndex + 1, $iLastPageIndex );
		$iDisplayIndex = ( $iPageCount > 0 ) ? $iPageIndex + 1 : 0;

		// Compute message range
		$arrMessages = $this->ListMessages( $iThreadID, true );
		$iMessageRangeStart = ( $iPageIndex * FORUM_MESSAGES_BY_PAGE );
		$iMessageRangeSize = 0;
		if ( $iPageCount > 0 ) {
			$iMessageRangeSize = FORUM_MESSAGES_BY_PAGE;
			if ( $bHasLastPage && ($iPageIndex == $iLastPageIndex) )
				$iMessageRangeSize = $iLastPageSize;
		}

		$hHTML->SetID( "ForumContent_Thread_Header" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "ForumContent_Thread_Header_Subject" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CH ForumProperties_CH_LeftText" );
			$hHTML->DivStart();
				$hHTML->Text( $strSubject );
			$hHTML->DivEnd();

			$hHTML->SetID( "ForumContent_Thread_Header_Commands" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CH ForumProperties_CH_RightContainer ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Thread_Header_Commands_ParentButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CH ForumProperties_CH_RightItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##$iBoardPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Parent" );
				$hHTML->DivEnd();

				// Logged user only
				if ( $bValidSession && !$bClosed ) {
					$hHTML->SetID( "ForumContent_Thread_Header_Commands_NewMessageButton" );
					$hHTML->SetStyleClass( "ForumContainer_Level3CH ForumProperties_CH_RightItem" );
					$hHTML->DivStart();
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayMessageNew'", "'$iThreadID##$iPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
						);
						$hHTML->InputButton( "New Message" );
					$hHTML->DivEnd();
				}
			$hHTML->DivEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_Thread_Messages" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			for( $i = 0; $i < $iMessageRangeSize; ++$i ) {
				$arrMessageInfos = $arrMessages[$iMessageRangeStart + $i];
				$iMessageID = $arrMessageInfos["MessageID"];
				$iUserID = $arrMessageInfos["UserID"];
				$strCreationTime = $arrMessageInfos["CreationTime"];
				$strLastAccess = $arrMessageInfos["LastAccess"];
				$strContent = $arrMessageInfos["Content"];
				
				$arrUserInfos = $hUser->GetInfosByID( $iUserID );
				$strUserLogin = $arrUserInfos["Login"];
				$strUserAvatar = $arrUserInfos["Avatar"];
				$strUserRank = $arrUserInfos["Rank"];
				$strUserMessageCount = $arrUserInfos["MessageCount"];
				
				if ( $strUserAvatar == "" ) {
					$iGender = $arrUserInfos["Gender"];
					$strUserAvatar = ( $iGender == USER_GENDER_FEMALE ) ? $hSystem->GetQueryHostRoot() . "/Medias/Avatars/_Default_Female.png"
																		: $hSystem->GetQueryHostRoot() . "/Medias/Avatars/_Default_Male.png";
				}

				$hHTML->SetID( "ForumContent_Thread_Messages_Message_$iMessageID" );
				$hHTML->SetStyleClass( "ForumContainer_Level2C ClearFix" );
				$hHTML->DivStart();
					$hHTML->TableStart( true );
						$hHTML->TableRowStart();
							$hHTML->SetStyleInline( "width:15%;" );
							$hHTML->TableCellStart( NULL, 2 );
								$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_UserViewCell ClearFix" );
								$hHTML->DivStart();
									$hHTML->SetStyleClass( "ForumContainer_Level3C ForumProperties_C_UserViewItem ForumProperties_C_UserLogin" );
									$hHTML->DivStart();
										$hHTML->Text( $strUserLogin );
									$hHTML->DivEnd();
									$hHTML->SetStyleClass( "ForumContainer_Level3C ForumProperties_C_UserViewItem" );
									$hHTML->DivStart();
										$hHTML->Image( $strUserAvatar, "<NoAvatar>", 192, 192 );
									$hHTML->DivEnd();
									$hHTML->SetStyleClass( "ForumContainer_Level3C ForumProperties_C_UserViewItem ForumProperties_C_UserRank" );
									$hHTML->DivStart();
										$hHTML->Text( "$strUserRank" );
									$hHTML->DivEnd();
									$hHTML->SetStyleClass( "ForumContainer_Level3C ForumProperties_C_UserViewItem" );
									$hHTML->DivStart();
										$hHTML->Text( "($strUserMessageCount Messages)" );
									$hHTML->DivEnd();
								$hHTML->DivEnd();
							$hHTML->TableCellEnd();
							$hHTML->SetStyleInline( "width:85%;" );
							$hHTML->TableCellStart();
								$hHTML->SetStyleClass( "ForumContainer_TableCell ForumProperties_MessageCell" );
								$hHTML->DivStart();
									$hHTML->Text( $strContent );
								$hHTML->DivEnd();
							$hHTML->TableCellEnd();
						$hHTML->TableRowEnd();
						$hHTML->TableRowStart();
							$hHTML->TableCellStart();
								$hHTML->SetStyleClass( "ForumContainer_TableCell ClearFix" );
								$hHTML->DivStart();
									$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftContainer ClearFix" );
									$hHTML->DivStart();
										$hHTML->SetStyleClass( "ForumContainer_Level4CF ForumProperties_CF_LeftItem" );
										$hHTML->DivStart();
											$hHTML->Text( "Created : $strCreationTime / Last Activity : $strLastAccess" );
										$hHTML->DivEnd();
									$hHTML->DivEnd();

									$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_RightContainer ClearFix" );
									$hHTML->DivStart();
										// Owning user only
										if ( $bValidSession && !$bClosed && ($iUserID == $hSession->GetUserID()) ) {
											$hHTML->SetStyleClass( "ForumContainer_Level4CF ForumProperties_CF_RightItem" );
											$hAJAX->SetEventTags (
												HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
												array( array( "'DisplayMessageEdit'", "'$iMessageID##$iPageIndex'" ),
													   array( "'DisplayThread'", "'$iThreadID'" ) )
											);
											$hHTML->InputButton( "Edit" );
										}
	
										////////////////////////////////////////////
										//$hHTML->InputButton( "PM" );
										//$hHTML->InputButton( "Mail" );
										//$hHTML->InputButton( "WebSite" );
										//$hHTML->InputButton( "Search" );
										//$hHTML->InputButton( "Quote" );
										//$hHTML->InputButton( "Report" );
									$hHTML->DivEnd();
								$hHTML->DivEnd();
							$hHTML->TableCellEnd();
						$hHTML->TableRowEnd();
					$hHTML->TableEnd();
				$hHTML->DivEnd();
			}
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_Thread_Footer" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "ForumContent_Thread_Footer_Navigation" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_LeftContainer ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Thread_Footer_Navigation_FirstPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayThread'", "'$iThreadID##0'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "First" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Thread_Footer_Navigation_PrevPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayThread'", "'$iThreadID##$iPrevPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Prev" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Thread_Footer_Navigation_PageIndex" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem ForumProperties_CF_PageIndex" );
				$hHTML->DivStart();
					$hHTML->Text( "$iDisplayIndex / $iPageCount" );
				$hHTML->DivEnd();
	
				$hHTML->SetID( "ForumContent_Thread_Footer_Navigation_NextPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayThread'", "'$iThreadID##$iNextPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Next" );
				$hHTML->DivEnd();
				
				$hHTML->SetID( "ForumContent_Thread_Footer_Navigation_LastPageButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_LeftItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayThread'", "'$iThreadID##$iLastPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Last" );
				$hHTML->DivEnd();

			$hHTML->DivEnd();

			$hHTML->SetID( "ForumContent_Thread_Footer_Commands" );
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightContainer ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "ForumContent_Thread_Footer_Commands_ParentButton" );
				$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_RightItem" );
				$hHTML->DivStart();
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayBoard'", "'$iBoardID##$iBoardPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Parent" );
				$hHTML->DivEnd();

				// Logged user only
				if ( $bValidSession && !$bClosed ) {
					$hHTML->SetID( "ForumContent_Thread_Footer_Commands_NewMessageButton" );
					$hHTML->SetStyleClass( "ForumContainer_Level3CF ForumProperties_CF_RightItem" );
					$hHTML->DivStart();
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayMessageNew'", "'$iThreadID##$iPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
						);
						$hHTML->InputButton( "New Message" );
					$hHTML->DivEnd();
				}
			$hHTML->DivEnd();

		$hHTML->DivEnd();
		
		if ( $bValidSession )
			$hSession->Close();
	}

		// Display routines : Edition
	private function _DisplayThreadNew( $iBoardID, $iPageIndex )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		
		$hHTML->SetID( "ForumContent_ThreadNew_Content" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "ForumContainer_Level2C ForumProperties_C_NewContent ForumProperties_C_SubjectInput" );
			$hHTML->DivStart();
				$hHTML->SetID( "ThreadNew_Subject" );
				$hHTML->InputText( "Thread Subject here ..." );
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "ForumContainer_Level2C ForumProperties_C_NewContent ForumProperties_C_MessageInput" );
			$hHTML->DivStart();
				$hHTML->SetID( "ThreadNew_Message" );
				$hHTML->InputTextArea( 16, 128, "Message Content here ..." );
			$hHTML->DivEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_ThreadNew_Commands" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightItem ForumProperties_CF_NewContentButton" );
			$hHTML->DivStart();
				$hHTML->SetID( "ThreadNew_CancelButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'DisplayBoard'", "'$iBoardID##$iPageIndex'" ), array( "'DisplayBoard'", "'$iBoardID'" ) )
				);
				$hHTML->InputButton( "Cancel" );
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightItem ForumProperties_CF_NewContentButton" );
			$hHTML->DivStart();
				$hHTML->SetID( "ThreadNew_ConfirmButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array (
						array( "'ThreadNew'", "'$iBoardID##'+".
											  "document.getElementById('ThreadNew_Subject').value.replace(/&/g,'%26')+'##'+".
											  "document.getElementById('ThreadNew_Message').value.replace(/&/g,'%26')" ),
						array( "'DisplayBoard'", "'$iBoardID'" )
					)
				);
				$hHTML->InputButton( "Confirm" );
			$hHTML->DivEnd();
		$hHTML->DivEnd();
	}
	private function _DisplayMessageNew( $iThreadID, $iPageIndex )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "ForumContent_MessageNew_Content" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "ForumContainer_Level2C ForumProperties_C_NewContent ForumProperties_C_MessageInput" );
			$hHTML->DivStart();
				$hHTML->SetID( "MessageNew_Message" );
				$hHTML->InputTextArea( 16, 128, "Enter your message here ..." );
			$hHTML->DivEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_MessageNew_Commands" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightItem ForumProperties_CF_NewContentButton" );
			$hHTML->DivStart();
				$hHTML->SetID( "MessageNew_CancelButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'DisplayThread'", "'$iThreadID##$iPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
				);
				$hHTML->InputButton( "Cancel" );
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightItem ForumProperties_CF_NewContentButton" );
			$hHTML->DivStart();
				$hHTML->SetID( "MessageNew_ConfirmButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array (
						array( "'MessageNew'", "'$iThreadID##'+".
											   "document.getElementById('MessageNew_Message').value.replace(/&/g,'%26')" ),
						array( "'DisplayThread'", "'$iThreadID'" )
					)
				);
				$hHTML->InputButton( "Confirm" );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayMessageEdit( $iMessageID, $iPageIndex )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$arrMessageInfos = $this->GetMessageInfos( $iMessageID );
		$iThreadID = $arrMessageInfos["ThreadID"];
		$strContent = $arrMessageInfos["Content"];

		$hHTML->SetID( "ForumContent_MessageEdit_Content" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "ForumContainer_Level2C ForumProperties_C_NewContent ForumProperties_C_MessageInput" );
			$hHTML->DivStart();
				$hHTML->SetID( "MessageEdit_Message" );
				$hHTML->InputTextArea( 16, 128, $strContent );
			$hHTML->DivEnd();
		$hHTML->DivEnd();

		$hHTML->SetID( "ForumContent_MessageEdit_Commands" );
		$hHTML->SetStyleClass( "ForumContainer_Level1C ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightItem ForumProperties_CF_NewContentButton" );
			$hHTML->DivStart();
				$hHTML->SetID( "MessageEdit_CancelButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'DisplayThread'", "'$iThreadID##$iPageIndex'" ), array( "'DisplayThread'", "'$iThreadID'" ) )
				);
				$hHTML->InputButton( "Cancel" );
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "ForumContainer_Level2CF ForumProperties_CF_RightItem ForumProperties_CF_NewContentButton" );
			$hHTML->DivStart();
				$hHTML->SetID( "MessageEdit_ConfirmButton" );
				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array (
						array( "'MessageEdit'", "'$iThreadID##$iPageIndex##$iMessageID##'+".
												"document.getElementById('MessageEdit_Message').value.replace(/&/g,'%26')" ),
						array( "'DisplayThread'", "'$iThreadID'" )
					)
				);
				$hHTML->InputButton( "Confirm" );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}

		// Display routines : Admin
	private function _DisplayAdmin( $iSectionID, $iBoardID, $iThreadID, $iMessageID )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "Forum_Admin" );
		$hHTML->DivStart();
			$arrSections = $this->ListSections( true );
			$iSectionCount = count( $arrSections );

			$arrSelectedSectionInfos = NULL;
			if ( $iSectionID != NULL )
				$arrSelectedSectionInfos = $this->GetSectionInfos( $iSectionID );

			$hHTML->SetID( "Forum_Admin_Sections" );
			$hHTML->DivStart();
				$hHTML->InputLabel( "ForumInput_Admin_Section_Select", "Section :" );
				$hHTML->SetID( "ForumInput_Admin_Section_Select" );
				$hHTML->InputComboListStart();
					$bSelected = ( $iSectionID == NULL );
					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayAdmin'", "'0##0##0##0'" ),
							   array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->ComboListItem( "*New Section*", NULL, $bSelected );

					for( $i = 0; $i < $iSectionCount; ++$i ) {
						$arrSectionInfos = $arrSections[$i];
						$iCurSectionID = $arrSectionInfos["SectionID"];
						$strName = $arrSectionInfos["Name"];

						$bSelected = ( $iSectionID == $iCurSectionID );
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayAdmin'", "'$iCurSectionID##0##0##0'" ),
								   array( "'DisplayRoot'", "''" ) )
						);
						$hHTML->ComboListItem( $strName, NULL, $bSelected );
					}
				$hHTML->InputComboListEnd();

				$mValue = 0;
				if ( $arrSelectedSectionInfos != NULL )
					$mValue = $arrSelectedSectionInfos["ShowIndex"];
				$hHTML->InputLabel( "ForumInput_Admin_Section_ShowIndex", "ShowIndex :" );
				$hHTML->SetID( "ForumInput_Admin_Section_ShowIndex" );
				$hHTML->InputNumber( $mValue );

				$mValue = "";
				if ( $arrSelectedSectionInfos != NULL )
					$mValue = $arrSelectedSectionInfos["Name"];
				$hHTML->InputLabel( "ForumInput_Admin_Section_Name", "Name :" );
				$hHTML->SetID( "ForumInput_Admin_Section_Name" );
				$hHTML->InputText( $mValue );

				$mValue = false;
				if ( $arrSelectedSectionInfos != NULL )
					$mValue = $arrSelectedSectionInfos["Public"];
				$hHTML->InputLabel( "ForumInput_Admin_Section_Public", "Public :" );
				$hHTML->SetID( "ForumInput_Admin_Section_Public" );
				$hHTML->InputCheckBox( NULL, $mValue );

				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'AdminSectionApply'", "'$iSectionID##'+".
													"document.getElementById('ForumInput_Admin_Section_ShowIndex').value+'##'+".
													"document.getElementById('ForumInput_Admin_Section_Name').value.replace(/&/g,'%26')+'##'+".
													"(document.getElementById('ForumInput_Admin_Section_Public').checked ? '1':'0')" ),
						   array( "'DisplayRoot'", "''" ) )
				);
				$hHTML->InputButton( "Apply" );

				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'DisplayAdmin'", "'0##0##0##0'" ),
						   array( "'DisplayRoot'", "''" ) )
				);
				$hHTML->InputButton( "Cancel" );

				$hAJAX->SetEventTags (
					HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
					array( array( "'AdminSectionDelete'", "'$iSectionID'" ),
						   array( "'DisplayRoot'", "''" ) )
				);
				$hHTML->InputButton( "Delete" );
			$hHTML->DivEnd();

			if ( $iSectionID != NULL ) {
				$arrBoards = $this->ListBoards( $iSectionID, true );
				$iBoardCount = count( $arrBoards );

				$arrSelectedBoardInfos = NULL;
				if ( $iBoardID != NULL )
					$arrSelectedBoardInfos = $this->GetBoardInfos( $iBoardID );

				$hHTML->SetID( "Forum_Admin_Boards" );
				$hHTML->DivStart();
					$hHTML->InputLabel( "ForumInput_Admin_Board_Select", "Board :" );
					$hHTML->SetID( "ForumInput_Admin_Board_Select" );
					$hHTML->InputComboListStart();
						$bSelected = ( $iBoardID == NULL );
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayAdmin'", "'$iSectionID##0##0##0'" ),
								   array( "'DisplayRoot'", "''" ) )
						);
						$hHTML->ComboListItem( "*New Board*", NULL, $bSelected );
	
						for( $i = 0; $i < $iBoardCount; ++$i ) {
							$arrBoardInfos = $arrBoards[$i];
							$iCurBoardID = $arrBoardInfos["BoardID"];
							$strName = $arrBoardInfos["Name"];
	
							$bSelected = ( $iBoardID == $iCurBoardID );
							$hAJAX->SetEventTags (
								HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
								array( array( "'DisplayAdmin'", "'$iSectionID##$iCurBoardID##0##0'" ),
									   array( "'DisplayBoard'", "'$iCurBoardID'" ) )
							);
							$hHTML->ComboListItem( $strName, NULL, $bSelected );
						}
					$hHTML->InputComboListEnd();

					$mValue = 0;
					if ( $arrSelectedBoardInfos != NULL )
						$mValue = $arrSelectedBoardInfos["ShowIndex"];
					$hHTML->InputLabel( "ForumInput_Admin_Board_ShowIndex", "ShowIndex :" );
					$hHTML->SetID( "ForumInput_Admin_Board_ShowIndex" );
					$hHTML->InputNumber( $mValue );

					$mValue = "";
					if ( $arrSelectedBoardInfos != NULL )
						$mValue = $arrSelectedBoardInfos["Name"];
					$hHTML->InputLabel( "ForumInput_Admin_Board_Name", "Name :" );
					$hHTML->SetID( "ForumInput_Admin_Board_Name" );
					$hHTML->InputText( $mValue );

					$mValue = "";
					if ( $arrSelectedBoardInfos != NULL )
						$mValue = $arrSelectedBoardInfos["Description"];
					$hHTML->InputLabel( "ForumInput_Admin_Board_Description", "Description :" );
					$hHTML->SetID( "ForumInput_Admin_Board_Description" );
					$hHTML->InputText( $mValue );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminBoardApply'", "'$iSectionID##$iBoardID##'+".
													"document.getElementById('ForumInput_Admin_Board_ShowIndex').value+'##'+".
													"document.getElementById('ForumInput_Admin_Board_Name').value.replace(/&/g,'%26')+'##'+".
													"document.getElementById('ForumInput_Admin_Board_Description').value.replace(/&/g,'%26')" ),
							   array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->InputButton( "Apply" );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayAdmin'", "'$iSectionID##0##0##0'" ),
							   array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->InputButton( "Cancel" );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminBoardDelete'", "'$iSectionID##$iBoardID'" ),
							   array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->InputButton( "Delete" );

					$hHTML->InputLabel( "ForumInput_Admin_Board_MoveSelect", "Move To Section :" );
					$hHTML->SetID( "ForumInput_Admin_Board_MoveSelect" );
					$hHTML->InputComboListStart();
						//////////////////////////////////////////////////////
					$hHTML->InputComboListEnd();

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminBoardMove'", "'$iBoardID##'+".
														  "document.getElementById('ForumInput_Admin_Board_MoveSelect').value" ),
							   array( "'DisplayRoot'", "''" ) )
					);
					$hHTML->InputButton( "Move" );
				$hHTML->DivEnd();
			}

			if ( $iBoardID != NULL ) {
				$arrStickyThreads = $this->ListThreads( $iBoardID, true, true );
				$arrThreads = $this->ListThreads( $iBoardID, false, true );
				$iStickyCount = count( $arrStickyThreads );
				$iThreadCount = count( $arrThreads );
				
				$arrSelectedThreadInfos = NULL;
				if ( $iThreadID != NULL )
					$arrSelectedThreadInfos = $this->GetThreadInfos( $iThreadID );

				$hHTML->SetID( "Forum_Admin_Threads" );
				$hHTML->DivStart();
					$hHTML->InputLabel( "ForumInput_Admin_Thread_Select", "Thread :" );
					$hHTML->SetID( "ForumInput_Admin_Thread_Select" );
					$hHTML->InputComboListStart();
						$bSelected = ( $iThreadID == NULL );
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##0##0'" ),
								   array( "'DisplayBoard'", "'$iBoardID'" ) )
						);
						$hHTML->ComboListItem( "*New Thread*", NULL, $bSelected );

						$hHTML->ComboListGroupStart( "Sticky Threads" );
							for( $i = 0; $i < $iStickyCount; ++$i ) {
								$arrThreadInfos = $arrStickyThreads[$i];
								$iCurThreadID = $arrThreadInfos["ThreadID"];
								$strSubject = $arrThreadInfos["Subject"];
		
								$bSelected = ( $iThreadID == $iCurThreadID );
								$hAJAX->SetEventTags (
									HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
									array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##$iCurThreadID##0'" ),
										   array( "'DisplayThread'", "'$iCurThreadID'" ) )
								);
								$hHTML->ComboListItem( $strSubject, NULL, $bSelected );
							}
						$hHTML->ComboListGroupEnd();
						$hHTML->ComboListGroupStart( "Threads" );
							for( $i = 0; $i < $iThreadCount; ++$i ) {
								$arrThreadInfos = $arrThreads[$i];
								$iCurThreadID = $arrThreadInfos["ThreadID"];
								$strSubject = $arrThreadInfos["Subject"];
		
								$bSelected = ( $iThreadID == $iCurThreadID );
								$hAJAX->SetEventTags (
									HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
									array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##$iCurThreadID##0'" ),
										   array( "'DisplayThread'", "'$iCurThreadID'" ) )
								);
								$hHTML->ComboListItem( $strSubject, NULL, $bSelected );
							}
						$hHTML->ComboListGroupEnd();
					$hHTML->InputComboListEnd();

					$mValue = "";
					if ( $arrSelectedThreadInfos != NULL )
						$mValue = $arrSelectedThreadInfos["Subject"];
					$hHTML->InputLabel( "ForumInput_Admin_Thread_Subject", "Subject :" );
					$hHTML->SetID( "ForumInput_Admin_Thread_Subject" );
					$hHTML->InputText( $mValue );

					$mValue = false;
					if ( $arrSelectedThreadInfos != NULL )
						$mValue = $arrSelectedThreadInfos["Sticky"];
					$hHTML->InputLabel( "ForumInput_Admin_Thread_Sticky", "Sticky :" );
					$hHTML->SetID( "ForumInput_Admin_Thread_Sticky" );
					$hHTML->InputCheckBox( NULL, $mValue );

					$mValue = false;
					if ( $arrSelectedThreadInfos != NULL )
						$mValue = $arrSelectedThreadInfos["Closed"];
					$hHTML->InputLabel( "ForumInput_Admin_Thread_Closed", "Closed :" );
					$hHTML->SetID( "ForumInput_Admin_Thread_Closed" );
					$hHTML->InputCheckBox( NULL, $mValue );

					$mValue = false;
					if ( $arrSelectedThreadInfos != NULL )
						$mValue = $arrSelectedThreadInfos["Archive"];
					$hHTML->InputLabel( "ForumInput_Admin_Thread_Archive", "Archive :" );
					$hHTML->SetID( "ForumInput_Admin_Thread_Archive" );
					$hHTML->InputCheckBox( NULL, $mValue );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminThreadApply'", "'$iSectionID##$iBoardID##$iThreadID##'+".
												 	"document.getElementById('ForumInput_Admin_Thread_Subject').value.replace(/&/g,'%26')+'##'+".
													"(document.getElementById('ForumInput_Admin_Thread_Sticky').checked ? '1':'0')+'##'+".
													"(document.getElementById('ForumInput_Admin_Thread_Closed').checked ? '1':'0')+'##'+".
													"(document.getElementById('ForumInput_Admin_Thread_Archive').checked ? '1':'0')" ),
							   array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Apply" );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##0##0'" ),
							   array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Cancel" );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminThreadDelete'", "'$iSectionID##$iBoardID##$iThreadID'" ),
							   array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Delete" );

					$hHTML->InputLabel( "ForumInput_Admin_Thread_MoveSelect", "Move To Board :" );
					$hHTML->SetID( "ForumInput_Admin_Thread_MoveSelect" );
					$hHTML->InputComboListStart();
						/////////////////////////////////////////////
					$hHTML->InputComboListEnd();

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminThreadMove'", "'$iSectionID##$iThreadID##'+".
														   "document.getElementById('ForumInput_Admin_Thread_MoveSelect').value" ),
							   array( "'DisplayBoard'", "'$iBoardID'" ) )
					);
					$hHTML->InputButton( "Move" );
				$hHTML->DivEnd();
			}

			if ( $iThreadID != NULL ) {
				$arrMessages = $this->ListMessages( $iThreadID, true );
				$iMessageCount = count( $arrMessages );
				
				$arrSelectedMessageInfos = NULL;
				if ( $iMessageID != NULL )
					$arrSelectedMessageInfos = $this->GetMessageInfos( $iMessageID );

				$hHTML->SetID( "Forum_Admin_Messages" );
				$hHTML->DivStart();
					$hHTML->InputLabel( "ForumInput_Admin_Message_Select", "Message :" );
					$hHTML->SetID( "ForumInput_Admin_Message_Select" );
					$hHTML->InputComboListStart();
						$bSelected = ( $iMessageID == NULL );
						$hAJAX->SetEventTags (
							HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
							array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##$iThreadID##0'" ),
								   array( "'DisplayThread'", "'$iThreadID'" ) )
						);
						$hHTML->ComboListItem( "*New Message*", NULL, $bSelected );
	
						for( $i = 0; $i < $iMessageCount; ++$i ) {
							$arrMessageInfos = $arrMessages[$i];
							$iCurMessageID = $arrMessageInfos["MessageID"];
							$strContent = $arrMessageInfos["Content"];
	
							$bSelected = ( $iMessageID == $iCurMessageID );
							$hAJAX->SetEventTags (
								HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
								array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##$iThreadID##$iCurMessageID'" ),
									   array( "'DisplayThread'", "'$iThreadID'" ) )
							);
							$hHTML->ComboListItem( $strContent, NULL, $bSelected );
						}
					$hHTML->InputComboListEnd();

					$mValue = "";
					if ( $arrSelectedMessageInfos != NULL )
						$mValue = $arrSelectedMessageInfos["Content"];
					$hHTML->InputLabel( "ForumInput_Admin_Message_Content", "Content :" );
					$hHTML->SetID( "ForumInput_Admin_Message_Content" );
					$hHTML->InputTextArea( 16, 128, $mValue );

					$mValue = false;
					if ( $arrSelectedMessageInfos != NULL )
						$mValue = $arrSelectedMessageInfos["Starter"];
					$hHTML->InputLabel( "ForumInput_Admin_Message_Starter", "Starter :" );
					$hHTML->SetID( "ForumInput_Admin_Message_Starter" );
					$hHTML->InputCheckBox( NULL, $mValue );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminMessageApply'", "'$iSectionID##$iBoardID##$iThreadID##$iMessageID##'+".
													"document.getElementById('ForumInput_Admin_Message_Content').value.replace(/&/g,'%26')+'##'+".
													"(document.getElementById('ForumInput_Admin_Message_Starter').checked ? '1':'0')" ),
							   array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Apply" );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'DisplayAdmin'", "'$iSectionID##$iBoardID##$iThreadID##0'" ),
							   array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Cancel" );

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminMessageDelete'", "'$iSectionID##$iBoardID##$iThreadID##$iMessageID'" ),
							   array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Delete" );
	
					$hHTML->SetID( "ForumInput_Admin_Message_MoveSelect" );
					$hHTML->InputComboListStart();
						/////////////////////////////////////////////
					$hHTML->InputComboListEnd();

					$hAJAX->SetEventTags (
						HTML_EVENT_ONMOUSECLICK, array( "ForumContent", "ForumHeader" ),
						array( array( "'AdminMessageMove'", "'$iSectionID##$iBoardID##$iThreadID##$iMessageID##'+".
														    "document.getElementById('ForumInput_Admin_Message_MoveSelect').value" ),
							   array( "'DisplayThread'", "'$iThreadID'" ) )
					);
					$hHTML->InputButton( "Move" );
				$hHTML->DivEnd();
			}

			// Reported messages /////////////////////////
		$hHTML->DivEnd();
	}

    // Members
}
?>