<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Search/_AJAX_SearchContent.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AJAX Request : SearchContent
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
require_once "../../Master.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Managers
$hLib = PHPLibrary::GetSingleton();

$hSystem = $hLib->System;

$hHTML = $hLib->HTML;
$hSQL = $hLib->SQL;
$hAJAX = $hLib->AJAX;

$hSearch = $hLib->Search;

/////////////////////////////////////////////////////////////////////////////////
// Build page

// Switch to SSL
$hSystem->EnsureSSL();

// Connect to database
$hSQL->ConnectTemporary();

// Get request parameters
$arrParameters = $hAJAX->GetRequestParameters( array( "SearchEvent", "SearchEventParam" ) );

$strSearchEvent = $arrParameters["SearchEvent"];
$strSearchEventParam = $arrParameters["SearchEventParam"];

// HTML output
$hHTML->DynamicTagStart();
	$hSearch->_DisplayContent( $strSearchEvent, $strSearchEventParam );
$hHTML->DynamicTagEnd();

// Disconnect from database
$hSQL->DisconnectTemporary();

?>