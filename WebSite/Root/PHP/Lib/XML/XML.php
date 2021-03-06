<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/XML/XML.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : XML definitions and interface
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
require_once "XMLNode.php";
require_once "XMLElement.php";
require_once "XMLAttribute.php";
require_once "XMLContent.php";
require_once "XMLDocument.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The XMLManager class
class XMLManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( XMLManager::$m_hInstance == NULL )
            XMLManager::$m_hInstance = new XMLManager();
        return XMLManager::$m_hInstance;
    }
    protected static $m_hInstance;

    // Constructor / Destructor
    public function __construct()
    {
		// nothing to do
	}
	public function __destruct()
    {
		// nothing to do
	}

	public function CreateDocument() {
		return new XMLDocument( new DomDocument() );
	}
}

?>