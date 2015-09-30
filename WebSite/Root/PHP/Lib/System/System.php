<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/System/System.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : System definitions and interface
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
	// OS Names
define( "SYSTEM_OS_UNKNOWN",   "Unknown" );
define( "SYSTEM_OS_WINDOWS",   "Windows" );
define( "SYSTEM_OS_MACINTOSH", "Macintosh" );
define( "SYSTEM_OS_UNIX",      "UNIX" );
define( "SYSTEM_OS_SUN" ,      "SunOS" );
define( "SYSTEM_OS_MOBILE",    "Mobile" );
define( "SYSTEM_OS_CONSOLE",   "Console" );

	// Browser Names
define( "SYSTEM_BROWSER_UNKNOWN",       "Unknown" );
define( "SYSTEM_BROWSER_FIREFOX",       "Mozilla Firefox" );
define( "SYSTEM_BROWSER_IE",            "Internet Explorer" );
define( "SYSTEM_BROWSER_CHROME",        "Google Chrome" );
define( "SYSTEM_BROWSER_OPERA",         "Opera" );
define( "SYSTEM_BROWSER_SAFARI",        "Safari" );
define( "SYSTEM_BROWSER_CURL",          "CURL" );
define( "SYSTEM_BROWSER_W3C",           "W3C Validator" );
define( "SYSTEM_BROWSER_BOT_EXALED",    "Exaled bot" );
define( "SYSTEM_BROWSER_BOT_GAMESPY",   "GameSpy bot" );
define( "SYSTEM_BROWSER_BOT_GIGABLAST", "GigaBlast bot" );
define( "SYSTEM_BROWSER_BOT_GOOGLE",    "Google bot" );
define( "SYSTEM_BROWSER_BOT_GOOGLEIMG", "Google-Image bot" );

	// MIME types
define( "MIME_ANY", "*/*" );

define( "MIME_TEXT",            "text/*" );
define( "MIME_TEXT_PLAIN",      "text/plain" );
define( "MIME_TEXT_RICH",       "text/enriched" );
define( "MIME_TEXT_RTF",        "text/rtf" );
define( "MIME_TEXT_HTML",       "text/html" );
define( "MIME_TEXT_XML",        "text/xml" );
define( "MIME_TEXT_CSS",        "text/css" );
define( "MIME_TEXT_JAVASCRIPT", "text/javascript" );

define( "MIME_AUDIO",        "audio/*" );
define( "MIME_AUDIO_MP4",    "audio/mp4" );
define( "MIME_AUDIO_AC3",    "audio/ac3" );
define( "MIME_AUDIO_MPEG",   "audio/mpeg" );
define( "MIME_AUDIO_MPEG4",  "audio/mpeg4-generic" );
define( "MIME_AUDIO_OGG",    "audio/ogg" );
define( "MIME_AUDIO_VORBIS", "audio/vorbis" );

define( "MIME_IMAGE",      "image/*" );
define( "MIME_IMAGE_JPEG", "image/jpeg" );
define( "MIME_IMAGE_PNG",  "image/png" );
define( "MIME_IMAGE_GIF",  "image/gif" );
define( "MIME_IMAGE_TIFF", "image/tiff" );

define( "MIME_VIDEO",        "video/*" );
define( "MIME_VIDEO_H264",   "video/H264" );
define( "MIME_VIDEO_MPEG",   "video/mpeg" );
define( "MIME_VIDEO_MPEG4",  "video/mpeg4-generic" );
define( "MIME_VIDEO_DV",     "video/DV" );
define( "MIME_VIDEO_QT",     "video/quicktime" );
define( "MIME_VIDEO_RAW",    "video/raw" );
define( "MIME_VIDEO_JPEG",   "video/JPEG" );
define( "MIME_VIDEO_JPEG2K", "video/jpeg2000" );
define( "MIME_VIDEO_MP4",    "video/mp4" );
define( "MIME_VIDEO_OGG",    "video/ogg" );

define( "MIME_APPLICATION",          "application/*" );
define( "MIME_APPLICATION_STREAM",   "application/octet-stream" );
define( "MIME_APPLICATION_HTTP",     "application/http" );
define( "MIME_APPLICATION_FORM_URL", "application/x-www-form-urlencoded" );
define( "MIME_APPLICATION_JS",       "application/javascript" );
define( "MIME_APPLICATION_SWF",      "application/x-shockwave-flash" );
define( "MIME_APPLICATION_XML",      "application/xml" );
define( "MIME_APPLICATION_XHTML",    "application/xhtml+xml" );
define( "MIME_APPLICATION_PDF",      "application/pdf" );
define( "MIME_APPLICATION_PS",       "application/postscript" );
define( "MIME_APPLICATION_MP4",      "application/mp4" );
define( "MIME_APPLICATION_MPEG4",    "application/mpeg4-generic" );
define( "MIME_APPLICATION_OGG",      "application/ogg" );
define( "MIME_APPLICATION_ZIP",      "application/zip" );

define( "MIME_MULTIPART",           "multipart/*" );
define( "MIME_MULTIPART_HEADER",    "multipart/header-set" );
define( "MIME_MULTIPART_FORM",      "multipart/form-data" );
define( "MIME_MULTIPART_DIGEST",    "multipart/digest" );
define( "MIME_MULTIPART_ENCRYPTED", "multipart/encrypted" );
define( "MIME_MULTIPART_MIXED",     "multipart/mixed" );

	// Charsets

	// Encodings

	// Languages

    // HTTP methods
define( "HTTP_METHOD_CONNECT", "connect" );
define( "HTTP_METHOD_DELETE",  "delete" );
define( "HTTP_METHOD_GET",     "get" );  // Client => Server, URL encoding
define( "HTTP_METHOD_HEAD",    "head" );
define( "HTTP_METHOD_OPTIONS", "options" );
define( "HTTP_METHOD_POST",    "post" ); // Client => Server, separate request
define( "HTTP_METHOD_PUT",     "put" );
define( "HTTP_METHOD_TRACE",   "trace" );

	// PHPInfos constants
define( "PHPINFO_ALL",           INFO_ALL );
define( "PHPINFO_GENERAL",       INFO_GENERAL );
define( "PHPINFO_CONFIGURATION", INFO_CONFIGURATION );
define( "PHPINFO_MODULES",       INFO_MODULES );
define( "PHPINFO_ENVIRONMENT",   INFO_ENVIRONMENT );
define( "PHPINFO_VARIABLES",     INFO_VARIABLES );

	// HTTP Header fields
define( "HTTP_HEADER_ACCEPT",             "Accept" );           // Value = <element> (',' <element>)*
                                                                // <element> = <MIMEType> (';q=' <float/quality/0;1>)?
define( "HTTP_HEADER_ACCEPTCHARSET",      "Accept-Charset" );   // Value = <element> (',' <element>)*
                                                                // <element> = <Charset> (';q=' <float/quality/0;1>)?
define( "HTTP_HEADER_ACCEPTENCODING",     "Accept-Encoding" );  // Value = <element> (',' <element>)*
                                                                // <element> = <Encoding> (';q=' <float/quality/0;1>)?
define( "HTTP_HEADER_ACCEPTLANGUAGE",     "Accept-Language" );  // Value = <element> (',' <element>)*
                                                                // <element> = <Language> (';q=' <float/quality/0;1>)?
//define( "HTTP_HEADER_ACCEPTRANGES",       "Accept-Ranges" );
define( "HTTP_HEADER_AGE",                "Age" );              // Value = <int/seconds/0;+inf>
//define( "HTTP_HEADER_ALLOW",              "Allow" );
//define( "HTTP_HEADER_AUTHORIZATION",      "Authorization" );
define( "HTTP_HEADER_CACHECONTROL",       "Cache-Control" );    // Value = <element> (',' <element>)*
                                                                // <element> = <CacheDirective> ( '=' (<fieldlist>|<int/seconds/0;+inf>) )?
															    // <fieldlist> = '"' <FieldName> (',' <FieldName>)* '"'
define( "HTTP_HEADER_CONNECTION",         "Connection" );       // Value = <ConnectionOption>
define( "HTTP_HEADER_CONTENTENCODING",    "Content-Encoding" ); // Value = 
define( "HTTP_HEADER_CONTENTLANGUAGE",    "Content-Language" );
define( "HTTP_HEADER_CONTENTLENGTH",      "Content-Length" );
define( "HTTP_HEADER_CONTENTLOCATION",    "Content-Location" );
define( "HTTP_HEADER_CONTENTMD5",         "Content-MD5" );
//define( "HTTP_HEADER_CONTENTRANGE",       "Content-Range" );
define( "HTTP_HEADER_CONTENTTYPE",        "Content-Type" );
define( "HTTP_HEADER_DATE",               "Date" );
//define( "HTTP_HEADER_ENTITYTAG",          "ETag" );
//define( "HTTP_HEADER_EXPECT",             "Expect" );
define( "HTTP_HEADER_EXPIRES",            "Expires" );
define( "HTTP_HEADER_FROM",               "From" );
define( "HTTP_HEADER_HOST",               "Host" );
//define( "HTTP_HEADER_IFMATCH",            "If-Match" );
//define( "HTTP_HEADER_IFMODIFIEDSINCE",    "If-Modified-Since" );
//define( "HTTP_HEADER_IFNONEMATCH",        "If-None-Match" );
//define( "HTTP_HEADER_IFRANGE",            "If-Range" );
//define( "HTTP_HEADER_IFUNMODIFIEDSINCE",  "If-Unmodified-Since" );
define( "HTTP_HEADER_LASTMODIFIED",       "Last-Modified" );
define( "HTTP_HEADER_LOCATION",           "Location" );
//define( "HTTP_HEADER_MAXFORWARDS",        "Max-Forwards" );
//define( "HTTP_HEADER_PROXYAUTHENTICATE",  "Proxy-Authenticate" );
//define( "HTTP_HEADER_PROXYAUTHORIZATION", "Proxy-Authorization" );
//define( "HTTP_HEADER_RANGE",              "Range" );
define( "HTTP_HEADER_REFERER",            "Referer" );
define( "HTTP_HEADER_RETRYAFTER",         "Retry-After" );
define( "HTTP_HEADER_SERVER",             "Server" );
//define( "HTTP_HEADER_TE",                 "TE" );
//define( "HTTP_HEADER_Trailer",            "Trailer" );
//define( "HTTP_HEADER_TRANSFERENCODING",   "Transfer-Encoding" );
//define( "HTTP_HEADER_UPGRADE",            "Upgrade" );
define( "HTTP_HEADER_USERAGENT",          "User-Agent" );
//define( "HTTP_HEADER_VARY",               "Vary" );
//define( "HTTP_HEADER_VIA",                "Via" );
//define( "HTTP_HEADER_WARNING",            "Warning" );
define( "HTTP_HEADER_WWWAUTHENTICATE",    "WWW-Authenticate" );
define( "HTTP_HEADER_XPOWEREDBY",         "X-Powered-By" );

	// Cache directives
define( "HTTP_HEADER_CACHE_PUBLIC",          "public" );
define( "HTTP_HEADER_CACHE_PRIVATE",         "private" );   // = "fieldlist" (optional)
define( "HTTP_HEADER_CACHE_NOCACHE",         "no-cache" );  // = "fieldlist" (response only, optional)
define( "HTTP_HEADER_CACHE_NOSTORE",         "no-store" );
define( "HTTP_HEADER_CACHE_NOTRANSFORM",     "no-transform" );
define( "HTTP_HEADER_CACHE_MAXAGE",          "max-age" );   // = value in seconds
define( "HTTP_HEADER_CACHE_MAXSTALE",        "max-stale" ); // = value in seconds (optional)
define( "HTTP_HEADER_CACHE_MINFRESH",        "min-fresh" ); // = value in seconds
define( "HTTP_HEADER_CACHE_SMAXAGE",         "s-maxage" );  // = value in seconds
define( "HTTP_HEADER_CACHE_MUSTREVALIDATE",  "must-revalidate" );
define( "HTTP_HEADER_CACHE_PROXYREVALIDATE", "proxy-revalidate" );
define( "HTTP_HEADER_CACHE_ONLYIFCACHED",    "only-if-cached" );

	// Connection options
define( "HTTP_HEADER_CONNECTION_CLOSE",     "close" );
define( "HTTP_HEADER_CONNECTION_KEEPALIVE", "keep-alive" );

/////////////////////////////////////////////////////////////////////////////////
// The SystemManager class
class SystemManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( SystemManager::$m_hInstance == NULL )
            SystemManager::$m_hInstance = new SystemManager();
        return SystemManager::$m_hInstance;
    }
    protected static $m_hInstance;

    // Constructor / Destructor
    public function __construct()
    {
        // Assertions
        assert_options( ASSERT_ACTIVE, true );
        assert_options( ASSERT_WARNING, true );
        assert_options( ASSERT_BAIL, true );
        assert_options( ASSERT_CALLBACK, "SystemManager::DefaultCallback" );
    }
    public function __destruct()
    {
		// nothing to do
	}

    // Assertions
    public function AssertToggle( $bEnable ) {
        assert_options( ASSERT_ACTIVE, $bEnable );
    }
    public function AssertUseWarnings( $bEnable ) {
        assert_options( ASSERT_WARNING, $bEnable );
    }
    public function AssertUseAbort( $bEnable ) {
        assert_options( ASSERT_BAIL, $bEnable );
    }

    public function SetAssertCallback( $pfCallback ) {
        if ( $pfCallback == NULL )
            assert_options( ASSERT_CALLBACK, "SystemManager::DefaultAssertCallback" );
        else
            assert_options( ASSERT_CALLBACK, $pfCallback );
    }

    public static function DefaultAssertCallback( $file, $line, $code ) {
        echo "\n\nAssertion Failed ('$file', '$line') : '$code'";
    }

	// UIDs
////////////////////////////////////////////////////////////////
	public function GenerateUID( $bStrong = true ) {
		$strBin = openssl_random_pseudo_bytes( 4, $bStrong );
		$strHex = bin2hex( $strBin );
		if ( $strHex[0] > '7' )
			$strHex[0] = '7';
		$strDec = hexdec( $strHex );
		return intval( $strDec );
	}
	
	// Randomness
	public function Random( $iByteCount, $bStrong = true ) {
		$strBin = openssl_random_pseudo_bytes( $iByteCount, $bStrong );
		return bin2hex( $strBin );
	}

    // Strings
	public function MakeHost( $strHostName, $iPort ) {
		return "$strHostName:$iPort";
	}
	public function MakeSiteRoot( $strProtocol, $strHostName, $iPort ) {
		return "$strProtocol://$strHostName:$iPort/";
	}
    public function MakeURI( $strPath, $strQuery = NULL ) {
        $strURL = "$strPath";
		if ( $strQuery != NULL )
			$strURL .= "?$strQuery";
		return $strURL;
    }
	public function MakeURL( $strProtocol, $strHostName, $iPort, $strPath, $strQuery = NULL ) {
        $strURL = "$strProtocol://$strHostName:$iPort/$strPath";
		if ( $strQuery != NULL )
			$strURL .= "?$strQuery";
		return $strURL;
    }

	public function StringEncodeArray( $arrArray, $strOuterGlue, $strInnerGlue )
	{
		if ( $arrArray == NULL )
			return NULL;
		if ( count($arrArray) == 0 )
			return "";

		$iArrayCount = count( $arrArray );
		for( $i = 0; $i < $iArrayCount; ++$i ) {
			if ( is_array( $arrArray[$i] ) )
				$arrArray[$i] = implode( $strInnerGlue, $arrArray[$i] );
		}
		return implode( $strOuterGlue, $arrArray );
	}
	public function StringDecodeArray( $strString, $strOuterGlue, $strInnerGlue )
	{
		if ( $strString == NULL )
			return NULL;
		if ( $strString == "" )
			return array();

		$arrArray = explode( $strOuterGlue, $strString );
		$iArrayCount = count( $arrArray );
		for( $i = 0; $i < $iArrayCount; ++$i ) {
			$iIndex = strpos( $arrArray[$i], $strInnerGlue );
			if ( $iIndex != false )
				$arrArray[$i] = explode( $strInnerGlue, $arrArray[$i] );
		}
		return $arrArray;
	}
	public function StringEncodeMap( $arrMap, $strOuterGlue, $strInnerGlue )
	{
		if ( $arrMap == NULL )
			return NULL;
		if ( count($arrMap) == 0 )
			return "";

		$strTemp = "";
		foreach( $arrMap as $key => $value ) {
			$strTemp .= "$key";
			if ( $value != NULL ) {
				$strTemp .= $strInnerGlue;
				$strTemp .= "$value";
			}
			$strTemp .= $strOuterGlue;
		}
		return substr( $strTemp, 0, -strlen($strOuterGlue) );
	}
	public function StringDecodeMap( $strString, $strOuterGlue, $strInnerGlue )
	{
		if ( $strString == NULL )
			return NULL;
		if ( $strString == "" )
			return array();

		$arrMap = array();
		$arrTemp = explode( $strOuterGlue, $strString );
		$iCount = count( $arrTemp );
		for( $i = 0; $i < $iCount; ++$i ) {
			$arrKeyVal = explode( $strInnerGlue, $arrTemp[$i] );
			if ( count($arrKeyVal) == 1 )
				$arrMap[$arrKeyVal[0]] = NULL;
			else if ( count($arrKeyVal) == 2 )
				$arrMap[$arrKeyVal[0]] = $arrKeyVal[1];
		}
		return $arrMap;
	}
	public function StringEncodeKeyVal( $arrKeys, $arrValues, $strOuterGlue, $strInnerGlue )
	{
		if ( $arrKeys == NULL )
			return NULL;
		if ( count($arrKeys) == 0 )
			return "";

		$strTemp = "";
		$iCount = count( $arrKeys );
		for( $i = 0; $i < $iCount; ++$i ) {
			$strTemp .= $arrKeys[$i];
			if ( $arrValues != NULL && $arrValues[$i] != NULL ) {
				$strTemp .= $strInnerGlue;
				$strTemp .= $arrValues[$i];
			}
			$strTemp .= $strOuterGlue;
		}
		return substr( $strTemp, 0, -strlen($strOuterGlue) );
	}
	public function StringDecodeKeyVal( $strString, $strOuterGlue, $strInnerGlue )
	{
		if ( $strParameters == NULL )
			return NULL;
		if ( $strParameters == "" )
			return array();

		$arrKeys = array();
		$arrValues = array();
		$arrTemp = explode( $strOuterGlue, $strString );
		$iCount = count( $arrTemp );
		for( $i = 0; $i < $iCount; ++$i ) {
			$arrKeyVal = explode( $strInnerGlue, $arrTemp[$i] );
			if ( count($arrKeyVal) == 1 ) {
				array_push( $arrKeys, $arrKeyVal[0] );
				array_push( $arrValues, NULL );
			} else if ( count($arrKeyVal) == 2 ) {
				array_push( $arrKeys, $arrKeyVal[0] );
				array_push( $arrValues, $arrKeyVal[1] );
			}
		}
		return array( "Keys" => $arrKeys, "Values" => $arrValues );
	}

	// Server Environment
    public function GetServerIdentifier() {
        return $_SERVER["SERVER_SOFTWARE"];
    }
	public function GetServerSignature() {
        return $_SERVER["SERVER_SIGNATURE"];
    }
	public function GetServerAddress() {
        return $_SERVER["SERVER_ADDR"];
    }
	public function GetServerName() {
        return $_SERVER["SERVER_NAME"];
    }
	public function GetServerPort() {
        return $_SERVER["SERVER_PORT"];
    }

	public function GetServerCGI() {
		return $_SERVER["GATEWAY_INTERFACE"];
	}
	public function GetServerProtocol() {
		return $_SERVER["SERVER_PROTOCOL"];
	}

	public function GetServerRoot() {
        return $_SERVER["DOCUMENT_ROOT"];
    }
	public function GetServerAdmin() {
        return $_SERVER["SERVER_ADMIN"];
    }

	// Client environment
	public function GetClientAddress() {
		return $_SERVER["REMOTE_ADDR"];
    }
	public function GetClientName() {
        return $_SERVER["REMOTE_HOST"];
    }
	public function GetClientPort() {
        return $_SERVER["REMOTE_PORT"];
    }

	public function GetClientOS()
	{
		static $arrWindows = array(
			"#windows\snt\s6\.2#i"         => "8",           "#windows\snt\s6\.1#i"         => "7",
			"#windows\s7#i"                => "7",           "#windows\snt\s6\.0;\swow64#i" => "Vista 64",
			"#windows\snt\s6\.0;\swin64#i" => "Vista 64",    "#windows\snt\s6\.0#i"         => "Vista",
			"#windows\snt\s5\.3#i"         => "Server 2003", "#windows\snt\s5\.2#i"         => "XP 64",
			"#windows\snt\s5\.1#i"         => "XP",          "#windows\sxp#i"               => "XP",
			"#nt\s5\.1#i"                  => "XP"
		);
		static $arrMacintosh = array(
			"#mac\sos\sx#i" => "MacOS X", "#mac_powerpc#i" => "MacOS 9"
		);
		static $arrUNIX = array(
			"#linux\sx86_64#i" => "Linux 64", "#linux#i"    => "Linux",
			"#libwww-fm#i"     => "Linux",    "#ubuntu#i"   => "Ubuntu",
			"#freebsd#i"       => "FreeBSD",  "#openbsd#i"  => "OpenBSD",
			"#netbsd#i"        => "NetBSD",   "#beos#i"     => "BeOS",
			"#fedora#i"        => "Fedora",   "#irix64#i"   => "Irix 64",
			"#irix#i"          => "Irix",     "#heritrix#i" => "Heritrix"
		);
		static $arrSun = array(
			"#sunos#i" => "Open Solaris", "#j2me#i" => "Java 2 Platform"
		);
		static $arrMobile = array(
			"#windows\sce#i" => "Windows CE",    "#iphone#i"     => "Mac iPhone",
			"#ipad#i"        => "Mac iPad",      "#ipod#i"       => "Mac iPod",
			"#android#i"     => "Linux Android", "#blackberry#i" => "BlackBerry OS"
		);
		static $arrConsole = array(
			"#nintendo\swii#i"   => "Nintendo Wii",              "#playstation\sportable#i" => "Sony PlayStation Portable",
			"#psp\s#i"           => "Sony PlayStation Portable", "#\spsp#i"                 => "Sony PlayStation Portable",
			"#playstation\s?3#i" => "Sony PlayStation 3"
		);

		$strUserAgent = $_SERVER["HTTP_USER_AGENT"];
		$strUserAgent = str_replace( "+", " ", $strUserAgent );
		$arrTemp = NULL;

		foreach( $arrWindows as $key => $value ) {
			if ( preg_match($key,$strUserAgent) )
				return array( "Name" => SYSTEM_OS_WINDOWS, "Version" => $value );
		}

		foreach( $arrMacintosh as $key => $value ) {
			if ( preg_match($key,$strUserAgent) )
				return array( "Name" => SYSTEM_OS_MACINTOSH, "Version" => $value );
		}

		foreach( $arrUNIX as $key => $value ) {
			if ( preg_match($key,$strUserAgent) )
				return array( "Name" => SYSTEM_OS_UNIX, "Version" => $value );
		}

		foreach( $arrSun as $key => $value ) {
			if ( preg_match($key,$strUserAgent) )
				return array( "Name" => SYSTEM_OS_SUN, "Version" => $value );
		}

		if ( preg_match("#windows\sphone\sos[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i", $strUserAgent, $arrTemp) )
			return array( "Name" => SYSTEM_OS_MOBILE, "Version" => "Windows PhoneOS ".$arrTemp[1] );
		if ( preg_match(   "#iphone\sos\s[\s\/]([0-9v]{1,7}(?:[\._][0-9a-z]{1,7}){0,7})#i", $strUserAgent, $arrTemp) )
			return array( "Name" => SYSTEM_OS_MOBILE, "Version" => "Mac iPhoneOS ".$arrTemp[1] );
		foreach( $arrMobile as $key => $value ) {
			if ( preg_match($key,$strUserAgent) )
				return array( "Name" => SYSTEM_OS_MOBILE, "Version" => $value );
		}

		foreach( $arrConsole as $key => $value ) {
			if ( preg_match($key,$strUserAgent) )
				return array( "Name" => SYSTEM_OS_CONSOLE, "Version" => $value );
		}

		return array( "Name" => SYSTEM_BROWSER_UNKNOWN, "Version" => NULL );
	}
	public function GetClientBrowser()
	{
		static $arrBrowsers = array (
			"#firefox[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_FIREFOX,
			   "#msie[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_IE,
			 "#chrome[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_CHROME,
			  "#opera[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_OPERA,
			 "#safari[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_SAFARI
		);
		static $arrInterfaces = array (
			         "#curl[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_CURL,
			"#w3c_validator[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_W3C
		);
		static $arrBots = array (
			         "#exabot[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_BOT_EXALED,
			             "#ng[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_BOT_EXALED,
			    "#gamespyhttp[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_BOT_GAMESPY,
			        "#gigabot[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_BOT_GIGABLAST,
			"#googlebot-image[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_BOT_GOOGLEIMG,
			      "#googlebot[\s\/]([0-9v]{1,7}(?:\.[0-9a-z]{1,7}){0,7})#i" => SYSTEM_BROWSER_BOT_GOOGLE
		);

		$strUserAgent = $_SERVER["HTTP_USER_AGENT"];
		$strUserAgent = str_replace( "+", " ", $strUserAgent );
		$arrTemp = NULL;

		foreach( $arrBrowsers as $key => $value ) {
			if ( preg_match($key,$strUserAgent,$arrTemp) )
				return array( "Name" => $value, "Version" => $arrTemp[1] );
		}

		foreach( $arrInterfaces as $key => $value ) {
			if ( preg_match($key,$strUserAgent,$arrTemp) )
				return array( "Name" => $value, "Version" => $arrTemp[1] );
		}

		if ( preg_match("#exabot-thumbnails#i", $strUserAgent) )
			return array( "Name" => SYSTEM_BROWSER_BOT_EXALED, "Version" => NULL );
		foreach( $arrBots as $key => $value ) {
			if ( preg_match($key,$strUserAgent,$arrTemp) )
				return array( "Name" => $value, "Version" => $arrTemp[1] );
		}

		return array( "Name" => SYSTEM_BROWSER_UNKNOWN, "Version" => NULL );
	}

    // PHP Environment
	public function PrintPHPInfos( $iShow = PHPINFO_ALL ) {
        return phpinfo( $iShow );
    }

    public function GetPHPVersionString() {
        return PHP_VERSION;
    }
    public function GetPHPVersionID() {
        return PHP_VERSION_ID;
    }
    public function GetPHPVersionMajor() {
        return PHP_MAJOR_VERSION;
    }
    public function GetPHPVersionMinor() {
        return PHP_MINOR_VERSION;
    }
    public function GetPHPVersionRelease() {
        return PHP_RELEASE_VERSION;
    }

	public function CheckPHPVersionString( $requiredVersion = "5.4.4" ) {
        return ( version_compare(PHP_VERSION, $requiredVersion) >= 0 );
    }
    public function CheckPHPVersionID( $requiredVersion = 50404 ) {
        return ( PHP_VERSION_ID >= $requiredVersion );
    }
	public function CheckPHPVersion( $requiredMajor = 5, $requiredMinor = 4, $requiredRelease = 4 ) {
        return ( ( PHP_MAJOR_VERSION >= $requiredMajor ) &&
                 ( PHP_MINOR_VERSION >= $requiredMinor ) &&
                 ( PHP_RELEASE_VERSION >= $requiredRelease ) );
    }

	public function GetAllPHPConfigValues() {
        return ini_get_all();
    }
    public function GetPHPConfigValue( $strName ) {
        return ini_get( $strName );
    }
    public function SetPHPConfigValue( $strName, $mValue ) {
        return ini_set( $strName, $mValue );
    }

	public function GetScriptTranslatedPath() {
        return $_SERVER["PATH_TRANSLATED"];
    }
	public function GetScriptFilePath() {
        return $_SERVER["SCRIPT_FILENAME"];
    }
    public function GetScriptPath() {
        return $_SERVER["PHP_SELF"];
    }

	public function IsCommandLine() {
		return ( PHP_SAPI == "cli" );
	}
	public function GetCommandLineArgs( &$arrArguments ) {
		global $argv;
		parse_str( implode("&", array_slice($argv, 1)), $arrArguments );
	}

	public function GetInitialTimeLimit() {
		return ini_get( "max_execution_time" );
	}
	public function SetTimeLimit( $iSeconds ) {
		set_time_limit( $iSeconds );
	}

	// Environment variables
	public function HasVariableENV( $strName ) {
        return isset( $_ENV[$strName] );
    }
    public function GetVariableENV( $strName ) {
        return $_ENV[$strName];
    }
    public function SetVariableENV( $strName, $mValue ) {
        $_ENV[$strName] = $mValue;
    }
	public function DestroyVariableENV( $strName ) {
        unset( $_ENV[$strName] );
    }

	// Input data
	public function GetQueryProtocol() {
        return $_SERVER["REQUEST_SCHEME"];
    }
	public function GetQueryMethod() {
        return $_SERVER["REQUEST_METHOD"];
    }

	public function GetQueryString() {
        return "https://" . $_SERVER["HTTP_HOST"] . $_SERVER["REQUEST_URI"];
	}
	public function GetQueryURL() {
        return "https://" . $_SERVER["HTTP_HOST"] . $_SERVER["SCRIPT_NAME"];
	}
	public function GetQueryHostRoot() {
        return "https://" . $_SERVER["HTTP_HOST"];
	}
	public function GetQueryHost() {
        return $_SERVER["HTTP_HOST"];
	}
	public function GetQueryURI() {
        return $_SERVER["REQUEST_URI"];
	}
	public function GetQueryFile() {
        return $_SERVER["SCRIPT_NAME"];
	}
	public function GetQueryParameters() {
        return $_SERVER["QUERY_STRING"];
	}

	public function HasVariableGET( $strName ) {
        return isset( $_GET[$strName] );
    }
    public function GetVariableGET( $strName ) {
        return $_GET[$strName];
    }
	public function HasVariablePOST( $strName ) {
        return isset( $_POST[$strName] );
    }
    public function GetVariablePOST( $strName ) {
        return $_POST[$strName];
    }

	public function HasCookie( $strName ) {
        return isset( $_COOKIE[$strName] );
    }
    public function GetCookie( $strName ) {
        return $_COOKIE[$strName];
    }
    public function CreateCookie( $strName, $strValue, $strDomain, $strPath, $iLifetime = 0,
                                  $bURLEncoding = true, $bSecure = false, $bHTTPOnly = false )
    {
        $iExpire = 0;
        if ( $iLifetime != 0 )
            $iExpire = ( time() + $iLifetime );

        $bOK = false;
        if ( $bURLEncoding )
            $bOK = setcookie( $strName, $strValue, $iExpire, $strPath, $strDomain, $bSecure, $bHTTPOnly );
        else
            $bOK = setrawcookie( $strName, $strValue, $iExpire, $strPath, $strDomain, $bSecure, $bHTTPOnly );
		assert( '$bOK' );
    }
    public function DestroyCookie( $strName )
    {
        $bOK = setcookie( $strName, "", time() - 3600 );
		assert( '$bOK' );
    }

	// Output buffering & redirection
    public function OutputBufferPush( $strCallback = NULL ) {
        $bOK = ob_start( $strCallback );
		assert( '$bOK' );
    }
    public function OutputBufferPop( $bFlush = true, $bGetData = false )
    {
        assert( '$this->OutputBufferIsActive()' );
        $strData = NULL;
        if ( $bGetData ) {
            if ( $bFlush )
                $strData = ob_get_flush();
            else
                $strData = ob_get_clean();
			assert( '$strData != NULL' );
        } else {
            if ( $bFlush )
				$bOK = ob_end_flush();
            else
                $bOK = ob_end_clean();
			assert( '$bOK' );
        }
        return $strData;
    }
    public function OutputBufferFlush() {
        assert( '$this->OutputBufferIsActive()' );
        ob_flush();
    }
    public function OutputBufferClear() {
        assert( '$this->OutputBufferIsActive()' );
        ob_clean();
    }
    public function OutputBufferSetAutoFlush( $bEnable = false ) {
        assert( '$this->OutputBufferIsActive()' );
        ob_implicit_flush( $bEnable );
    }

    public function OutputBufferIsActive() {
        return ( ob_get_level() > 0 );
    }
    public function OutputBufferGetLevel() {
        return ob_get_level();
    }
    public function OutputBufferGetLength() {
        assert( '$this->OutputBufferIsActive()' );
        return ob_get_length();
    }
    public function OutputBufferGetData() {
        assert( '$this->OutputBufferIsActive()' );
        return ob_get_contents();
    }

	public function OutputRedirectStart( $strFilename )
	{
		assert( '$this->m_strOutputRedirectFile == NULL' );

		$this->m_strOutputRedirectFile = $strFilename;
		$this->OutputBufferPush();
	}
	public function OutputRedirectEnd()
	{
		assert( '$this->m_strOutputRedirectFile != NULL' );

		$strOutput = $this->OutputBufferPop( false, true );

		$hFile = fopen( $this->m_strOutputRedirectFile, "a" );
		fwrite( $hFile, $strOutput );
		fclose( $hFile );

		$this->m_strOutputRedirectFile = NULL;
	}

	// HTTP support
	public function GetRequestHeader( $strHeaderField ) {
		$strKeyName = "HTTP_" . str_replace( "-", "_", strtoupper($strHeaderField) );
		if ( !isset($_SERVER[$strKeyName]) )
			return NULL;
		return $_SERVER[$strKeyName];
	}

	public function GetRequestTime( $bMicroseconds = false ) {
        if ( $bMicroseconds )
            return $_SERVER["REQUEST_TIME_FLOAT"];
        else
            return $_SERVER["REQUEST_TIME"];
    }

	public function GetResponseCode() {
		return http_response_code();
	}
	public function SetResponseCode( $iCode ) {
		http_response_code( $iCode );
	}

	public function IsResponseHeaderSent() {
		return headers_sent();
	}
	public function GetResponseHeader() {
		return headers_list();
	}

	public function AddResponseHeader( $strHeaderField, $strValue, $bReplace = true ) {
		header( "$strHeaderField: $strValue", $bReplace );
	}
	public function RemoveResponseHeader( $strField ) {
		header_remove( $strField );
	}
	public function RemoveResponseHeaderAll() {
		header_remove();
	}

	public function HeaderRedirect( $strAbsoluteURI ) {
		header( "Location: $strAbsoluteURI" );
	}
	public function HeaderDisableCache() {
		header( "Cache-Control: no-cache" );
		header( "Pragma: no-cache" );
		$strDate = date( DATE_RFC1123, time() - 3600 );
		header( "Expires: $strDate" );
	}
	public function HeaderStartDownload( $strURL ) {
		header( "Content-Type: application/octet-stream" );
		header( "Content-Disposition: attachment; filename=$strURL" );
		readfile( $strURL );
	}
	// More helpers with header operations ... ///////////////////////

	// SSL support
	public function HasSSL() {
        return ( $_SERVER["HTTPS"] == "on" );
    }
	public function EnsureSSL() {
		if( $_SERVER["HTTPS"] != "on" ) {
			header( "Location: https://" . $_SERVER["HTTP_HOST"] . $_SERVER["REQUEST_URI"] );
			exit();
		}
	}

	// SMTP/POP3/IMAP support
	public function EMail( $strDestination, $strSubject, $strMessage, $strFrom, $strHeaders = "" ) {
		$bOK = mail( $strDestination, $strSubject, $strMessage, "From: $strFrom" . "\r\n" . $strHeaders );
		assert( '$bOK' );
	}

	// Members
	private $m_strOutputRedirectFile;
}

?>