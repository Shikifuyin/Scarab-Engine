<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/System/HTTP.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : HTTP specification support
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Note that this class handles RESPONSE header building.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// The HTTPManager class
class HTTPManager
{
	// Singleton interface
    public static function GetSingleton() {
        if ( HTTPManager::$m_hInstance == NULL )
            HTTPManager::$m_hInstance = new HTTPManager();
        return HTTPManager::$m_hInstance;
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

	// Header management
	public function HeaderAccept( $arrTypes, $arrPriorities = NULL ) {
		$hSystem = SystemManager::GetSingleton();
		$strTypeSelection = $hSystem->StringEncodeKeyVal( $arrTypes, $arrPriorities, ",", ";q=" );
		header( "Accept: $strTypeSelection" );
	}
	public function HeaderAcceptCharset( $arrCharsets, $arrPriorities = NULL ) {
		$hSystem = SystemManager::GetSingleton();
		$strCharsetSelection = $hSystem->StringEncodeKeyVal( $arrCharsets, $arrPriorities, ",", ";q=" );
		header( "Accept-Charset: $strCharsetSelection" );
	}
	public function HeaderAcceptEncoding( $arrEncodings, $arrPriorities = NULL ) {
		$hSystem = SystemManager::GetSingleton();
		$strEncodingSelection = $hSystem->StringEncodeKeyVal( $arrEncodings, $arrPriorities, ",", ";q=" );
		header( "Accept-Encoding: $strEncodingSelection" );
	}
	public function HeaderAcceptLanguage( $arrLanguages, $arrPriorities = NULL ) {
		$hSystem = SystemManager::GetSingleton();
		$strLanguageSelection = $hSystem->StringEncodeKeyVal( $arrLanguages, $arrPriorities, ",", ";q=" );
		header( "Accept-Language: $strLanguageSelection" );
	}
	public function HeaderAcceptRanges( $bUseByteRanges = false ) {
		$strRanges = $bUseByteRanges ? "bytes" : "none";
		header( "Accept-Ranges: $strRanges" );
	}
	public function HeaderAge( $iSeconds ) {
		header( "Age: $iSeconds" );
	}
	public function HeaderAllow( $strMethods ) {
		header( "Allow: $strMethods" );
	}
	public function HeaderAuthorization( $strAuthData ) {
		header( "Authorization: $strAuthData" );
	}
	public function HeaderCacheControl( $arrCacheDirectives, $arrParameters = NULL ) {
		$hSystem = SystemManager::GetSingleton();
		$strCacheDirectives = $hSystem->StringEncodeKeyVal( $arrCacheDirectives, $arrParameters, ",", "=" );
		header( "Cache-Control: $strCacheDirectives" );
		if ( strpos($strCacheDirectives, "no-cache") != false ) // Compatibility
			header( "Pragma: no-cache" );                       // issue fix ...
	}
	public function HeaderConnection( $strOptions ) {
		header( "Connection: $strOptions" );
	}
	public function HeaderContentEncoding( $strEncodings ) {
		header( "Content-Encoding: $strEncodings" );
	}
	public function HeaderContentLanguage( $strLanguages ) {
		header( "Content-Language: $strLanguages" );
	}
	public function HeaderContentLength( $iByteLength ) {
		header( "Content-Length: $iByteLength" );
	}
	public function HeaderContentLocation( $strURI ) {
		header( "Content-Location: $strURI" );
	}
	public function HeaderContentMD5( $strHash ) {
		header( "Content-MD5: $strHash" );
	}
	public function HeaderContentRange( $iFirstByte = NULL, $iLastByte = NULL, $iTotalLength = NULL ) {
		$strRange = ($iFirstByte == NULL) ? "*" : ( ($iLastByte == NULL) ? "*" : "$iFirstByte-$iLastByte" );
		$strSize = ($iTotalLength == NULL) ? "*" : "$iTotalLength";
		header( "Content-Range: bytes $strRange/$strSize" );
	}
	public function HeaderContentType( $strType ) {
		header( "Content-Type: $strType" );
	}
	public function HeaderDate( $iTimeStamp ) {
		$strDate = date( DATE_RFC1123, $iTimeStamp );
		header( "Date: $strDate" );
	}
	public function HeaderEntityTag( $strETag ) {
		header( "ETag: $strETag" );
	}
	public function HeaderExpect( $arrExpectFields = NULL, $arrExpectValues = NULL ) {
		$strExpect = "100-continue";
		if ( $arrExpectFields != NULL && $arrExpectValues != NULL ) {
			$hSystem = SystemManager::GetSingleton();
			$strExpect = $hSystem->StringEncodeKeyVal( $arrExpectFields, $arrExpectValues, ";", "=" );
		}
		header( "Expect: $strExpect" );
	}
	public function HeaderExpires( $iTimeStamp ) {
		$strDate = date( DATE_RFC1123, $iTimeStamp );
		header( "Expires: $strDate" );
	}
	public function HeaderFrom( $strEMail ) {
		header( "From: $strEMail" );
	}
	public function HeaderHost( $strHostName, $iPort = NULL ) {
		if ( $iPort != NULL ) $strHostName .= ":$iPort";
		header( "Host: $strHostName" );
	}
	public function HeaderIfMatch( $strETags = NULL ) {
		if ( $strETags == NULL ) $strETags = "*";
		header( "If-Match: $strETags" );
	}
	public function HeaderIfModifiedSince( $iTimeStamp ) {
		$strDate = date( DATE_RFC1123, $iTimeStamp );
		header( "If-Modified-Since: $strDate" );
	}
	public function HeaderIfNoneMatch( $strETags = NULL ) {
		if ( $strETags == NULL ) $strETags = "*";
		header( "If-None-Match: $strETags" );
	}
	public function HeaderIfRange( $mTimeStampOrETag ) {
		$strIfRange = "";
		if ( is_int($mTimeStampOrETag) )
			$strIfRange = date( DATE_RFC1123, $mTimeStampOrETag );
		else {
			assert( 'is_string($mTimeStampOrETag)' );
			$strIfRange = $mTimeStampOrETag;
		}
		header( "If-Range: $strIfRange" );
	}
	public function HeaderIfUnmodifiedSince( $iTimeStamp ) {
		$strDate = date( DATE_RFC1123, $iTimeStamp );
		header( "If-Unmodified-Since: $strDate" );
	}
	public function HeaderLastModified( $iTimeStamp ) {
		$strDate = date( DATE_RFC1123, $iTimeStamp );
		header( "Last-Modified: $strDate" );
	}
	public function HeaderLocation( $strAbsoluteURI ) {
		header( "Location: $strAbsoluteURI" );
	}
	public function HeaderMaxForwards( $iMaxForwards ) {
		header( "Max-Forwards: $iMaxForwards" );
	}
	public function HeaderProxyAuthenticate( $strAuthChallenges ) {
		header( "Proxy-Authenticate: $strAuthChallenges" );
	}
	public function HeaderProxyAuthorization( $strAuthData ) {
		header( "Proxy-Authorization: $strAuthData" );
	}
	public function HeaderRange( $arrRangeMins, $arrRangeMaxs ) {
		$hSystem = SystemManager::GetSingleton();
		$strRanges = $hSystem->StringEncodeKeyVal( $arrRangeMins, $arrRangeMaxs, ",", "-" );
		header( "Range: bytes=$strRanges" );
	}
	public function HeaderReferer( $strURI ) {
		header( "Referer: $strURI" );
	}
	public function HeaderRetryAfter( $iTimeStamp, $bAbsolute = false ) {
		$strRetry = $bAbsolute ? date(DATE_RFC1123, $iTimeStamp) : "$iTimeStamp";
		header( "Retry-After: $strRetry" );
	}
	public function HeaderServer( $strServerInfos ) {
		header( "Server: $strServerInfos" );
	}
	public function HeaderTE( $arrEncodings, $arrPriorities = NULL ) {
		$hSystem = SystemManager::GetSingleton();
		$strEncodingSelection = $hSystem->StringEncodeKeyVal( $arrEncodings, $arrPriorities, ",", ";q=" );
		header( "TE: $strEncodingSelection" );
	}
	public function HeaderTrailer( $strFields ) {
		header( "Trailer: $strFields" );
	}
	public function HeaderTransferEncoding( $strEncodings ) {
		header( "Transfer-Encoding: $strEncodings" );
	}
	public function HeaderUpgrade( $strProtocols ) {
		header( "Upgrade: $strProtocols" );
	}
	public function HeaderUserAgent( $strUserAgent ) {
		header( "User-Agent: $strUserAgent" );
	}
	public function HeaderVary( $strFields ) {
		header( "Vary: $strFields" );
	}
	public function HeaderVia( $arrProtocols, $arrHosts, $arrComments = NULL ) {
		if ( $arrComments != NULL ) {
			$iCount = count( $arrHosts );
			for( $i = 0; $i < $iCount; ++$i ) {
				$strComment = $arrComments[$i];
				if ( $strComment != NULL )
					$arrHosts[i] .= " $strComment";
			}
		}
		$hSystem = SystemManager::GetSingleton();
		$strVia = $hSystem->StringEncodeKeyVal( $arrProtocols, $arrHosts, ",", " " );
		header( "Via: $strVia" );
	}
	public function HeaderWarning( $arrCodes, $arrHosts, $arrTexts, $arrTimeStamps = NULL ) {
		$iCount = count( $arrCodes );
		for( $i = 0; $i < $iCount; ++$i ) {
			$strHost = $arrHosts[$i];
			$arrCodes[i] .= " $strHost";
		}
		if ( $arrTimeStamps != NULL ) {
			for( $i = 0; $i < $iCount; ++$i ) {
				if ( $arrTimeStamps[$i] != NULL ) {
					$strDate = date( DATE_RFC1123, $arrTimeStamps[$i] );
					$arrTexts[i] .= " $strDate";
				}
			}
		}
		$hSystem = SystemManager::GetSingleton();
		$strWarning = $hSystem->StringEncodeKeyVal( $arrCodes, $arrTexts, ",", " " );
		header( "Warning: $strWarning" );
	}
	public function HeaderWWWAuthenticate( $strAuthChallenges ) {
		header( "WWW-Authenticate: $strAuthChallenges" );
	}

	// Helpers

	// Members
}

?>