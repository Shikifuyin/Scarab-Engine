<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/System/Network.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Network operations toolbox
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
// The NetworkManager class
class NetworkManager
{
	// Singleton interface
    public static function GetSingleton() {
        if ( NetworkManager::$m_hInstance == NULL )
            NetworkManager::$m_hInstance = new NetworkManager();
        return NetworkManager::$m_hInstance;
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

	// Networking
	public function AddressToString( $iIPAddress ) {
        return long2ip( $iIPAddress );
    }
    public function AddressToInt( $strIPAddress ) {
        return ip2long( $strIPAddress );
    }

    public function GetProtocolName( $iProtocolID ) {
        $strProtocolName = getprotobynumber( $iProtocolID );
        if ( !$strProtocolName )
            return NULL;
        return $strProtocolName;
    }
    public function GetProtocolID( $strProtocolName ) {
        $iProtocolID = getprotobyname( $strProtocolName );
        if ( !$iProtocolID )
            return NULL;
        return $iProtocolID;
    }

    public function GetPortService( $strProtocolName, $iPortNumber ) {
        $strService = getservbyport( $iPortNumber, $strProtocolName );
        if ( !$strService )
            return NULL;
        return $strService;
    }
    public function GetPortNumber( $strProtocolName, $strService ) {
        $iPortNumber = getservbyname( $strService, $strProtocolName );
        if ( !$iPortNumber )
            return NULL;
        return $iPortNumber;
    }

    public function GetHostName( $strIPAddress ) {
        $strHostName = gethostbyaddr( $strIPAddress );
        if ( !$strHostName )
            return NULL;
        return $strHostName;
    }
    public function GetHostAddress( $strHostName ) {
        $strIPAddress = gethostbyname( $strHostName );
        if ( !$strIPAddress )
            return NULL;
        return $strIPAddress;
    }

	// Helpers

	// Members
}

?>