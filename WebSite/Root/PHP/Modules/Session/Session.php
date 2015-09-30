<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Session/Session.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Modules, Session
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
define( "SESSION_LIFETIME", 72 ); // Must be > Cookie's lifetime !

define( "SESSION_COOKIE_NAME",     "ScarabEngine_Cookie_SessionID" );
define( "SESSION_COOKIE_LIFETIME", 48 );

/////////////////////////////////////////////////////////////////////////////////
// The SessionManager class
class SessionManager
{
	// Singleton interface
	public static function GetSingleton() {
		if ( SessionManager::$m_hInstance == NULL )
			SessionManager::$m_hInstance = new SessionManager();
		return SessionManager::$m_hInstance;
	}
	protected static $m_hInstance;

	// Constructor / Destructor
	public function __construct()
	{
		$this->_GenerateTables();

		$this->m_iSessionID = NULL;
		$this->m_iUserID = NULL;
		$this->m_strSessionData = NULL;
		$this->m_arrSessionVariables = NULL;
	}
	public function __destruct()
    {
		// nothing to do
	}

	// Session management
	public function Create( $iUserID )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		// Check loaded session
		if ( $this->_IsLoaded() ) {
			if ( $this->m_iUserID == $iUserID )
				return true;
			$this->_Unload( true );
		}

		// Check existing session
		$iSessionID = $this->_RetrieveSessionID( $iUserID );
		if ( $iSessionID != NULL ) {
			$this->_Load( $iSessionID );
			return true;
		}

		// Make a cleanup pass before creation
		$iLimit = ( time() - (SESSION_LIFETIME * 3600) );
		
		$hSQL->Delete( "_PHP_Sessions",
					   "LastAccess<?", array( $iLimit ) );

		// Create & load new session
		$iSessionID = $hSystem->GenerateUID();
		$strUserIP = $hSystem->GetClientAddress();
		
		$hSQL->Insert( "_PHP_Sessions", array( "SessionID"   => $iSessionID,
											   "UserID"      => $iUserID,
											   "SessionData" => "",
											   "UserIP"      => $strUserIP ) );

		$hSystem->CreateCookie( SESSION_COOKIE_NAME, "$iSessionID", $hSystem->GetQueryHost(), "/",
								(SESSION_COOKIE_LIFETIME * 3600), true, true, true );

		$this->_Load( $iSessionID );
		return true;
	}
	public function Destroy( $iUserID = NULL )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		$iSessionID = NULL;
		$bDestroyCookie = NULL;

		// Retrieve SessionID
		if ( $iUserID == NULL ) {
			$bDestroyCookie = true;

			if ( !($this->_IsLoaded()) )
				return false;
			$iSessionID = $this->m_iSessionID;

			$this->_Unload( false );
		} else {
			$bDestroyCookie = false;

			if ( $this->_IsLoaded() ) {
				if ( $this->m_iUserID == $iUserID ) {
					$bDestroyCookie = true;
					$this->_Unload( false );
				}
			}

			$iSessionID = $this->_RetrieveSessionID( $iUserID );
			if ( $iSessionID == NULL )
				return false;
		}

		// Destroy session
		if ( $bDestroyCookie )
			$hSystem->DestroyCookie( SESSION_COOKIE_NAME );

		$hSQL->Delete( "_PHP_Sessions",
					   "SessionID=?", array( $iSessionID ) );
		return true;
	}

	public function Open( $iUserID = NULL )
	{
		// Check loaded session
		if ( $this->_IsLoaded() ) {
			if ( $this->m_iUserID == $iUserID )
				return true;
			$this->_Unload( true );
		}

		// Retrieve SessionID
		$iSessionID = $this->_RetrieveSessionID( $iUserID );
		if ( $iSessionID == NULL )
			return false;

		// Open session
		$this->_Load( $iSessionID );
		return true;
	}
	public function Close( $bCommit = true )
	{
		// Close session
		if ( $this->_IsLoaded() )
			$this->_Unload( $bCommit );
	}

	public function IsOpened() {
		return $this->_IsLoaded();
	}
	public function GetID() {
		assert( '$this->m_iSessionID != NULL' );
		return $this->m_iSessionID;
	}
	public function GetUserID() {
		assert( '$this->m_iSessionID != NULL' );
		return $this->m_iUserID;
	}
	public function IsAdmin() {
		assert( '$this->m_iSessionID != NULL' );
		return ( $this->m_iUserID == USER_ADMIN_ID );
	}

	// Session variables
	public function HasVariable( $strName ) {
		assert( '$this->m_arrSessionVariables != NULL' );
		return isset( $this->m_arrSessionVariables[$strName] );
	}
	public function GetVariable( $strName ) {
		assert( '$this->m_arrSessionVariables != NULL' );
		return $this->m_arrSessionVariables[$strName];
	}
	public function SetVariable( $strName, $mValue ) {
		assert( '$this->m_arrSessionVariables != NULL' );
		$this->m_arrSessionVariables[$strName] = $mValue;
	}
	public function DestroyVariable( $strName ) {
		assert( '$this->m_arrSessionVariables != NULL' );
		$this->m_arrSessionVariables[$strName] = NULL;
		unset( $this->m_arrSessionVariables[$strName] );
	}

	public function EnumAllVariables() {
		assert( '$this->m_arrSessionVariables != NULL' );
		return array_keys( $this->m_arrSessionVariables );
	}
	public function DestroyAllVariables() {
		assert( '$this->m_arrSessionVariables != NULL' );
		$this->m_arrSessionVariables = array();
	}

	// Helpers
	private function _GenerateTables()
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->ConnectTemporary();

		// Sessions table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Sessions ( ";
		$strQuery .= "SessionID int UNSIGNED NOT NULL, ";
		$strQuery .= "UserID int UNSIGNED NOT NULL, ";
		$strQuery .= "LastAccess timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP, ";
		$strQuery .= "SessionData text NOT NULL, ";
		$strQuery .= "UserIP char(16) NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( SessionID ), ";
		$strQuery .= "FOREIGN KEY ( UserID ) REFERENCES _PHP_Users( UserID ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );
		
		$hSQL->DisconnectTemporary();
	}

	private function _RetrieveSessionID( $iUserID = NULL )
	{
		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		// Retrieve from DB
		if ( $iUserID != NULL ) {
			$hSQL->Select( "_PHP_Sessions", array( "SessionID" ),
					   	   "UserID=?", array( $iUserID ) );
	
			if ( $hSQL->GetRowCount() == 0 ) {
				$hSQL->FreeResults();
				return NULL;
			}
			assert( '$hSQL->GetRowCount() == 1' );
	
			$arrSessionRow = $hSQL->FetchRow();
			$hSQL->FreeResults();
	
			return $arrSessionRow["SessionID"];
		}

		// Retrieve from Cookie
		$bValidCookie = $hSystem->HasCookie( SESSION_COOKIE_NAME );
		if ( !$bValidCookie )
			return NULL;

		$iSessionID = intval( $hSystem->GetCookie( SESSION_COOKIE_NAME ) );

		// Trap dead cookies
		$hSQL->Select( "_PHP_Sessions", array( "UserID" ),
					   "SessionID=?", array( $iSessionID ) );
		
		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();

			$hSystem->DestroyCookie( SESSION_COOKIE_NAME );
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$hSQL->FreeResults();
		
		return $iSessionID;
	}

	private function _IsLoaded() {
		return ( $this->m_iSessionID != NULL );
	}
	private function _Load( $iSessionID )
	{
		assert( '$this->m_iSessionID == NULL' );

		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Select( "_PHP_Sessions", array( "UserID", "SessionData" ),
					   "SessionID=?", array( $iSessionID ) );

		assert( '$hSQL->GetRowCount() == 1' );
		$arrSessionRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		$this->m_iSessionID = $iSessionID;
		$this->m_iUserID = $arrSessionRow["UserID"];
		$this->m_strSessionData = $arrSessionRow["SessionData"];
		$this->m_arrSessionVariables = $hSystem->StringDecodeMap( $this->m_strSessionData, "&", "=" );
	}
	private function _Unload( $bCommit )
	{
		assert( '$this->m_iSessionID != NULL' );

		$hSystem = SystemManager::GetSingleton();
		$hSQL = SQLManager::GetSingleton();

		if ( $bCommit ) {
			$this->m_strSessionData = $hSystem->StringEncodeMap( $this->m_arrSessionVariables, "&", "=" );

			$hSQL->Update( "_PHP_Sessions", array( "SessionData" => $this->m_strSessionData ),
						   "SessionID=?", array( $this->m_iSessionID ) );
		}

		$this->m_iSessionID = NULL;
		$this->m_iUserID = NULL;
		$this->m_strSessionData = NULL;
		$this->m_arrSessionVariables = NULL;
	}

	// Members
	private $m_iSessionID;
	private $m_iUserID;
	private $m_strSessionData;
	private $m_arrSessionVariables;
}

?>