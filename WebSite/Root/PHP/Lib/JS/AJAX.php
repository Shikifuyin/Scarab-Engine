<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Lib/JS/AJAX.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AJAX requests support
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : Note : - You really should use <div> tags only, even if I allow
//                     any tag having an ID to be used here ...
//                     Behaviour with iframes, spans, ... is undefined !
//                     - Server-side UpdateTag calls MUST be placed in the tag's
//                       body itself ...
//              TODO : Finalize browser history support ...
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions
	// Request types
define( "AJAX_REQUEST_HTML", "html" );
define( "AJAX_REQUEST_XML", "xml" );

/////////////////////////////////////////////////////////////////////////////////
// The AJAXManager class
class AJAXManager
{
	// Singleton interface
    public static function GetSingleton() {
        if ( AJAXManager::$m_hInstance == NULL )
            AJAXManager::$m_hInstance = new AJAXManager();
        return AJAXManager::$m_hInstance;
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

	// Tags management
	public function Initialize()
	{
//		$hHTML = HTMLManager::GetSingleton();

//		$hHTML->HeadScriptStart( MIME_TEXT_JAVASCRIPT );
//			$hHTML->HeadScriptCode( "window.onpopstate = function(event) {" );
//			$hHTML->HeadScriptCode( "    var hState = event.state;" );
//			$hHTML->HeadScriptCode( "    var RequestFunction = window[hState.RequestFunction];" );
//			$hHTML->HeadScriptCode( "    var RequestParams = hState.RequestParams;" );
//			$hHTML->HeadScriptCode( "    RequestFunction.apply( RequestFunction, RequestParams );" );
//			$hHTML->HeadScriptCode( "};" );
//		$hHTML->HeadScriptEnd();
	}
	public function GenerateTagRequest( $strTagID, $strRequestURL, $arrParameterKeys = NULL, $strAsyncCallback = "",
										$strRequestType = AJAX_REQUEST_HTML, $strRequestXMLFilter = "", $bAsynchronous = true )
	{
		$hHTML = HTMLManager::GetSingleton();

		$strParameterKeys = "";
		if ( $arrParameterKeys != NULL )
			$strParameterKeys = implode( ",", $arrParameterKeys );

		if ( $strRequestType == AJAX_REQUEST_XML ) {
			if ( $strRequestXMLFilter == "" )
				$strRequestXMLFilter = $this->_GenerateDefaultXMLFilter();
		}
		$strAsynchronous = $bAsynchronous ? "true" : "false";

		$strQueryPOST = "AJAX_Request=1&AJAX_TagID=$strTagID";
		if ( $arrParameterKeys != NULL ) {
			$iCount = count( $arrParameterKeys );
			for( $i = 0; $i < $iCount; ++$i ) {
				$strKey = $arrParameterKeys[$i];
				$strQueryPOST .= "&$strKey=\"+$strKey+\"";
			}
		}

		$hHTML->HeadScriptStart( MIME_TEXT_JAVASCRIPT );
			if ( $strRequestType == AJAX_REQUEST_XML ) {
				$hHTML->HeadScriptCode( "function _AJAXRequest_XMLFilter_$strTagID( hDocXML ) {" );
				$hHTML->HeadScriptCode( "    var strOutputHTML = \"\";" );
				$hHTML->HeadScriptCode( "    $strRequestXMLFilter" );
				$hHTML->HeadScriptCode( "    return strOutputHTML;" );
				$hHTML->HeadScriptCode( "}" );
			}

			if ( $bAsynchronous ) {
				$hHTML->HeadScriptCode( "function _AJAXRequest_Callback_$strTagID( localRequest, $strParameterKeys ) {" );
				$hHTML->HeadScriptCode( "    if ( localRequest.readyState == 4 && localRequest.status == 200 ) {" );
				if ( $strRequestType == AJAX_REQUEST_XML ) {
					$hHTML->HeadScriptCode( "        var strText = _AJAXRequest_XMLFilter_$strTagID( localRequest.responseXML );" );
					$hHTML->HeadScriptCode( "        document.getElementById(\"$strTagID\").innerHTML = strText;" );
				} else
					$hHTML->HeadScriptCode( "        document.getElementById(\"$strTagID\").innerHTML = localRequest.responseText;" );
				$hHTML->HeadScriptCode( "        _AJAXRequest_AsyncCallback_$strTagID( $strParameterKeys );" );
				$hHTML->HeadScriptCode( "    }" );
				$hHTML->HeadScriptCode( "}" );
				$hHTML->HeadScriptCode( "function _AJAXRequest_AsyncCallback_$strTagID( $strParameterKeys ) {" );
				$hHTML->HeadScriptCode( "    $strAsyncCallback" );
				$hHTML->HeadScriptCode( "}" );
			}

			$hHTML->HeadScriptCode( "function _AJAXRequest_$strTagID( $strParameterKeys ) {" );
			$hHTML->HeadScriptCode( "    var hRequest = false;" );
			$hHTML->HeadScriptCode( "    if ( window.XMLHttpRequest ) {" );
			$hHTML->HeadScriptCode( "        hRequest = new XMLHttpRequest();" );
			$hHTML->HeadScriptCode( "        hRequest.overrideMimeType( 'text/xml' );" );
			$hHTML->HeadScriptCode( "    } else if ( window.ActiveXObject ) {" );
			$hHTML->HeadScriptCode( "        hRequest = new ActiveXObject( 'Microsoft.XMLHTTP' );" );
			$hHTML->HeadScriptCode( "    }" );

//			$hHTML->HeadScriptCode( "    var hState = {" );
//			$hHTML->HeadScriptCode( "        RequestFunction: '_AJAXRequest_$strTagID'," );
//			$hHTML->HeadScriptCode( "        RequestParams: [$strParameterKeys]" );
//			$hHTML->HeadScriptCode( "    };" );
//			$hHTML->HeadScriptCode( "    window.history.pushState( hState, '' );" );

			if ( $bAsynchronous ) {
				$hHTML->HeadScriptCode( "    hRequest.onreadystatechange = function() {" );
				$hHTML->HeadScriptCode( "        _AJAXRequest_Callback_$strTagID( hRequest, $strParameterKeys );" );
				$hHTML->HeadScriptCode( "    };" );
			}
			$hHTML->HeadScriptCode( "    hRequest.open( \"post\", \"$strRequestURL\", $strAsynchronous );" );
			$hHTML->HeadScriptCode( "    hRequest.setRequestHeader( \"Cache-Control\", \"no-cache\" );" );
			$hHTML->HeadScriptCode( "    hRequest.setRequestHeader( \"Content-Type\", \"application/x-www-form-urlencoded\" );" );
			$hHTML->HeadScriptCode( "    hRequest.send( \"$strQueryPOST\" );" );
			if ( !$bAsynchronous ) {
			    if ( $strRequestType == AJAX_REQUEST_XML ) {
					$hHTML->HeadScriptCode( "    var strText = _AJAXRequest_XMLFilter_$strTagID( hRequest.responseXML );" );
					$hHTML->HeadScriptCode( "    document.getElementById(\"$strTagID\").innerHTML = strText;" );
				} else
					$hHTML->HeadScriptCode( "    document.getElementById(\"$strTagID\").innerHTML = hRequest.responseText;" );
			}
			$hHTML->HeadScriptCode( "}" );
		$hHTML->HeadScriptEnd();
	}

	public function GetRequestParameters( $arrParameterKeys )
	{
		$hSystem = SystemManager::GetSingleton();
		
		$strTagID = NULL;
		if ( $hSystem->HasVariablePOST("AJAX_Request") && $hSystem->GetVariablePOST("AJAX_Request") == 1 )
			$strTagID = $hSystem->GetVariablePOST("AJAX_TagID");
		else
			return NULL;

		$arrParameters = array();
		$iCount = count( $arrParameterKeys );
		for( $i = 0; $i < $iCount; ++$i ) {
			$strKey = $arrParameterKeys[$i];
			$arrParameters[$strKey] = $hSystem->GetVariablePOST( $strKey );
		}

		return $arrParameters;
	}

	public function UpdateTag( $bServerSide, $strTagID, $arrParameters = NULL )
	{
		if ( $bServerSide ) {
			// Server side update using include
			$arrOldPOST = $_POST;
			$_POST = array();

			$_POST["AJAX_Request"] = 1;
			$_POST["AJAX_TagID"] = $strTagID;

			if ( $arrParameters != NULL ) {
				foreach( $arrParameters as $key => $value ) {
					if ( is_string($value) )
						$_POST[$key] = trim( $value, "'" );
					else
						$_POST[$key] = $value;
				}
			}

			include( "_AJAX_$strTagID.php" );

			$_POST = $arrOldPOST;

			return true;
		} else {
			// Client side update using Javascript
			$hHTML = HTMLManager::GetSingleton();

			$strParameters = "";
			if ( $arrParameters != NULL )
				$strParameters = implode( ",", $arrParameters );

			$hHTML->HeadScriptStart( MIME_TEXT_JAVASCRIPT );
				$hHTML->HeadScriptCode( "_AJAXRequest_$strTagID($strParameters);" );
			$hHTML->HeadScriptEnd();
			
			return true;
		}
	}

	public function SetEventTag( $strEvent, $strTagID, $arrParameterValues = NULL, $strCallbackBefore = NULL, $strCallbackAfter = NULL, $iDelay = 0 )
	{
		$hHTML = HTMLManager::GetSingleton();

		$strParameters = "";
		if ( $arrParameterValues != NULL )
			$strParameters = implode( ",", $arrParameterValues );

		$strScript = "";
		
		if ( $strCallbackBefore != NULL )
			$strScript .= "$strCallbackBefore";
		
		if ( $iDelay > 0 )
			$strScript .= "setTimeout( function() { _AJAXRequest_$strTagID($strParameters); }, $iDelay );";
		else
			$strScript .= "_AJAXRequest_$strTagID($strParameters);";

		if ( $strCallbackAfter != NULL )
			$strScript .= "$strCallbackAfter";

		$hHTML->SetEventCallback( $strEvent, $strScript );
	}
	public function SetEventTags( $strEvent, $arrTagIDs, $arrParameterValues = NULL, $strCallbackBefore = NULL, $strCallbackAfter = NULL,
																																    $iDelay = 0 )
	{
		$hHTML = HTMLManager::GetSingleton();

		$strScript = "";
		
		if ( $strCallbackBefore != NULL )
			$strScript .= "$strCallbackBefore";

		$iCount = count( $arrTagIDs );
		for( $i = 0; $i < $iCount; ++$i ) {
			$strTagID = $arrTagIDs[$i];

			$strTagParameters = "";
			if ( $arrParameterValues != NULL ) {
				$arrTagParameters = $arrParameterValues[$i];
				if ( $arrTagParameters != NULL )
					$strTagParameters = implode( ",", $arrTagParameters );
			}

			if ( $iDelay > 0 )
				$strScript .= "setTimeout( function() { _AJAXRequest_$strTagID($strTagParameters); }, $iDelay );";
			else
				$strScript .= "_AJAXRequest_$strTagID($strTagParameters);";
		}

		if ( $strCallbackAfter != NULL )
			$strScript .= "$strCallbackAfter";

		$hHTML->SetEventCallback( $strEvent, $strScript );
	}

	// Helpers

	// Members
}

?>