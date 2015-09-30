<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Master.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Master header for the PHP library & modules
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Upon MySQL 5.6 release, change all timestamps to datetime
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
require_once "Lib/System/System.php";
require_once "Lib/System/Network.php";
require_once "Lib/System/HTTP.php";

require_once "Lib/HTML/HTML.php";
require_once "Lib/CSS/CSS.php";
require_once "Lib/SQL/SQL.php";
require_once "Lib/JS/AJAX.php";
require_once "Lib/XML/XML.php";

require_once "Modules/Session/User.php";
require_once "Modules/Session/Account.php";
require_once "Modules/Session/Session.php";
require_once "Modules/Forum/Forum.php";
require_once "Modules/CodeDoc/CodeDoc.php";
require_once "Modules/Search/Search.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The PHPLibrary class
class PHPLibrary
{
    // Singleton interface
    public static function GetSingleton() {
        if ( PHPLibrary::$m_hInstance == NULL )
            PHPLibrary::$m_hInstance = new PHPLibrary();
        return PHPLibrary::$m_hInstance;
    }
    protected static $m_hInstance;
    
    // Constructor / Destructor
    public function __construct()
    {
        // Build all managers
        $this->System = SystemManager::GetSingleton();
		$this->Network = NetworkManager::GetSingleton();
		$this->HTTP = HTTPManager::GetSingleton();

        $this->HTML = HTMLManager::GetSingleton();
		$this->CSS = CSSManager::GetSingleton();
        $this->SQL = SQLManager::GetSingleton();
		$this->AJAX = AJAXManager::GetSingleton();
		$this->XML = XMLManager::GetSingleton();

		$this->User = UserManager::GetSingleton();
		$this->Account = AccountManager::GetSingleton();
        $this->Session = SessionManager::GetSingleton();
		$this->Forum = ForumManager::GetSingleton();
		$this->CodeDoc = CodeDocManager::GetSingleton();
		$this->Search = SearchManager::GetSingleton();
    }
	public function __destruct()
    {
		// nothing to do
	}

    // Getters

    // Methods

    // Members
    public $System;
	public $Network;
	public $HTTP;

    public $HTML;
    public $CSS;
    public $SQL;
    public $AJAX;
    public $XML;

	public $User;
	public $Account;
	public $Session;
	public $Forum;
	public $CodeDoc;
	public $Search;
}

?>