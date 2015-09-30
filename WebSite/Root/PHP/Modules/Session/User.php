<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Session/User.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : User management interface
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
define( "USER_ADMIN_ID", 0xfaded69d );

	// Genders
define( "USER_GENDER_FEMALE", 0 );
define( "USER_GENDER_MALE",   1 );

	// Ranks
define( "USER_RANK_NEWBIE", 	 "Newbie" );
define( "USER_RANK_INTERESTED",  "Interested" );
define( "USER_RANK_STUDENT", 	 "Student" );
define( "USER_RANK_DEBUGGING",   "Debugging" );
define( "USER_RANK_AMATEUR", 	 "Amateur" );
define( "USER_RANK_SPEAKINGC",   "Speaking C" );
define( "USER_RANK_SPEAKINGCPP", "Speaking C++" );
define( "USER_RANK_VETERAN", 	 "Veteran" );
define( "USER_RANK_SPEAKINGASM", "Speaking x86" );
define( "USER_RANK_PRO", 	 	 "Pro" );
define( "USER_RANK_WHISPERER",   "Machine Whisperer" );
define( "USER_RANK_WATCHER",     "Machine Watcher" );
define( "USER_RANK_ENGINEER",    "Reverse Engineer" );
define( "USER_RANK_CODEGURU", 	 "Code Guru" );
define( "USER_RANK_CODEEXPERT",  "Code Expert" );
define( "USER_RANK_CODEMASTER",  "Code Master" );
define( "USER_RANK_CODESENSEI",  "Code Sensei" );

/////////////////////////////////////////////////////////////////////////////////
// The UserManager class
class UserManager
{
	// Singleton interface
	public static function GetSingleton() {
		if ( UserManager::$m_hInstance == NULL )
			UserManager::$m_hInstance = new UserManager();
		return UserManager::$m_hInstance;
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

	// User management
	public function Create( $strActivationSeed,
							$strLogin, $strPassword, $strEMail, $strFirstName, $strLastName,
							$strBirthday, $iGender, $strCountry, $strAvatar = "" )
	{
		$hSQL = SQLManager::GetSingleton();
		
		if ( strlen($strActivationSeed) != 64 )
			return NULL;

		$bValidUser = $this->_CanCreate( $strLogin, $strEMail );
		if ( !$bValidUser )
			return NULL;

		$hSQL->Insert( "_PHP_Users", array( "Activation"   => $strActivationSeed,
											"Login"        => $strLogin,
											"Password"     => sha1($strPassword),
											"EMail" 	   => $strEMail,
											"FirstName"    => $strFirstName,
											"LastName" 	   => $strLastName,
											"Birthday"     => $strBirthday,
											"Gender"       => $iGender,
											"Country"      => $strCountry,
											"Avatar"	   => $strAvatar,
											"Rank"         => USER_RANK_NEWBIE,
											"MessageCount" => 0 ) );
		return $hSQL->GetAutoIncrement();
	}
	public function Activate( $strActivationSeed ) {
		$hSQL = SQLManager::GetSingleton();

		if ( strlen($strActivationSeed) != 64 )
			return false;

		$hSQL->Update( "_PHP_Users", array( "Activation" => "0" ),
					   "Activation='?'", array( $strActivationSeed ) );
   
		return true;
	}
	public function Destroy( $iUserID )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Delete( "_PHP_Users",
					   "UserID=?", array( $iUserID ) );

		return true;
	}

	public function UpdatePassword( $iUserID, $strPassword )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Update( "_PHP_Users", array( "Password" => sha1($strPassword) ),
					   "UserID=?", array( $iUserID ) );
	}
	public function UpdateEMail( $iUserID, $strEMail )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Select( "_PHP_Users", array( "UserID" ),
					   "EMail='?'", array( $strEMail ) );
					   
		if ( $hSQL->GetRowCount() > 0 ) {
			$hSQL->FreeResults();
			return false;
		}
		$hSQL->FreeResults();

		$hSQL->Update( "_PHP_Users", array( "EMail" => $strEMail ),
					   "UserID=?", array( $iUserID ) );
   
		return true;
	}
	public function UpdateName( $iUserID, $strFirstName, $strLastName )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_Users", array( "FirstName" => $strFirstName,
											"LastName"  => $strLastName ),
					   "UserID=?", array( $iUserID ) );
	}
	public function UpdateInfos( $iUserID, $strBirthday, $iGender, $strCountry  )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_Users", array( "Birthday" => $strBirthday,
											"Gender"   => $iGender,
											"Country"  => $strCountry ),
					   "UserID=?", array( $iUserID ) );
	}
	public function UpdateAvatar( $iUserID, $strAvatar )
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->Update( "_PHP_Users", array( "Avatar" => $strAvatar ),
					   "UserID=?", array( $iUserID ) );
	}
	public function UpdateMessageCount( $iUserID, $iMessages )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$arrUserInfos = $this->GetInfosByID( $iUserID );
		$iMessageCount = $arrUserInfos["MessageCount"] + $iMessages;

		$hSQL->Update( "_PHP_Users", array( "MessageCount" => $iMessageCount,
											"Rank" => $this->_GetRank($iMessageCount) ),
					   "UserID=?", array( $iUserID ) );
	}

	public function ListAll( $bGetInfos = false )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$arrKeys = $bGetInfos ? NULL : array( "UserID" );
		$hSQL->Select( "_PHP_Users", $arrKeys );

		$arrUsers = array();
		$arrUserRow = $hSQL->FetchRow();
		while( $arrUserRow != NULL ) {
			if ( $bGetInfos )
				array_push( $arrUsers, $arrUserRow );
			else
				array_push( $arrUsers, $arrUserRow["UserID"] );
			$arrUserRow = $hSQL->FetchRow();
		}
		$hSQL->FreeResults();

		return $arrUsers;
	}

	public function GetInfosByID( $iUserID )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Select( "_PHP_Users", NULL,
					   "UserID=?", array( $iUserID ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrUserRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrUserRow;
	}
	public function GetInfosByLogin( $strLogin )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Select( "_PHP_Users", NULL,
					   "Login='?'", array( $strLogin ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrUserRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrUserRow;
	}
	public function GetInfosByEMail( $strEMail )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Select( "_PHP_Users", NULL,
					   "EMail='?'", array( $strEMail ) );

		if ( $hSQL->GetRowCount() == 0 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		assert( '$hSQL->GetRowCount() == 1' );
		$arrUserRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		return $arrUserRow;
	}

	// Authentification
	public function Authentify( $strLogin, $strPassword )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$strHash = sha1( $strPassword );
		
		$hSQL->Select( "_PHP_Users", array( "UserID", "Activation", "Login", "Password" ),
					   "Login='?' AND Password='?'", array( $strLogin, $strHash ) );

		if ( $hSQL->GetRowCount() != 1 ) {
			$hSQL->FreeResults();
			return NULL;
		}
		$arrUserRow = $hSQL->FetchRow();
		$hSQL->FreeResults();

		if ( $arrUserRow["Activation"] != "0" )
			return NULL;
		if ( $arrUserRow["Login"] != $strLogin )
			return NULL;
		if ( $arrUserRow["Password"] != $strHash )
			return NULL;
		return $arrUserRow["UserID"];
	}

	// Helpers
	private function _GenerateTables()
	{
		$hSQL = SQLManager::GetSingleton();

		$hSQL->ConnectTemporary();

		// Users table
		$strQuery = "CREATE TABLE IF NOT EXISTS _PHP_Users ( ";
		$strQuery .= "UserID int UNSIGNED NOT NULL AUTO_INCREMENT, ";
		$strQuery .= "Activation char(64) NOT NULL, ";
		$strQuery .= "Login char(64) NOT NULL, ";
		$strQuery .= "Password char(64) NOT NULL, ";
		$strQuery .= "EMail char(64) NOT NULL, ";
		$strQuery .= "LastAccess timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP, ";
		$strQuery .= "FirstName char(64) NOT NULL, ";
		$strQuery .= "LastName char(64) NOT NULL, ";
		$strQuery .= "Birthday date NOT NULL, ";
		$strQuery .= "Gender tinyint UNSIGNED NOT NULL, ";
		$strQuery .= "Country char(64) NOT NULL, ";
		$strQuery .= "Avatar tinytext NOT NULL, ";
		$strQuery .= "Rank char(32) NOT NULL, ";
		$strQuery .= "MessageCount int UNSIGNED NOT NULL, ";
		$strQuery .= "PRIMARY KEY ( UserID ), ";
		$strQuery .= "UNIQUE ( Login ), ";
		$strQuery .= "UNIQUE ( EMail ) ";
		$strQuery .= ")";

		$bOK = $hSQL->Query( $strQuery );
		assert( '$bOK' );
		
		$hSQL->DisconnectTemporary();
	}

	private function _CanCreate( $strLogin, $strEMail )
	{
		$hSQL = SQLManager::GetSingleton();
		
		$hSQL->Select( "_PHP_Users", array( "UserID" ),
					   "Login='?' OR EMail='?'", array( $strLogin, $strEMail ) );

		if ( $hSQL->GetRowCount() > 0 ) {
			$hSQL->FreeResults();
			return false;
		}

		$hSQL->FreeResults();
		return true;
	}

	private function _GetRank( $iMessageCount ) {
		if ( $iMessageCount >= 3000 ) return USER_RANK_CODESENSEI;
		if ( $iMessageCount >= 2000 ) return USER_RANK_CODEMASTER;
		if ( $iMessageCount >= 1500 ) return USER_RANK_CODEEXPERT;
		if ( $iMessageCount >= 1100 ) return USER_RANK_CODEGURU;
		if ( $iMessageCount >=  800 ) return USER_RANK_ENGINEER;
		if ( $iMessageCount >=  600 ) return USER_RANK_WATCHER;
		if ( $iMessageCount >=  400 ) return USER_RANK_WHISPERER;
		if ( $iMessageCount >=  300 ) return USER_RANK_PRO;
		if ( $iMessageCount >=  200 ) return USER_RANK_SPEAKINGASM;
		if ( $iMessageCount >=  150 ) return USER_RANK_VETERAN;
		if ( $iMessageCount >=  100 ) return USER_RANK_SPEAKINGCPP;
		if ( $iMessageCount >=   75 ) return USER_RANK_SPEAKINGC;
		if ( $iMessageCount >=   50 ) return USER_RANK_AMATEUR;
		if ( $iMessageCount >=   30 ) return USER_RANK_DEBUGGING;
		if ( $iMessageCount >=   20 ) return USER_RANK_STUDENT;
		if ( $iMessageCount >=   10 ) return USER_RANK_INTERESTED;
		return USER_RANK_NEWBIE;
	}

	// Members
}

?>