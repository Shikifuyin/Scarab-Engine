<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/CodeDoc/_AJAX_CodeDocHeader.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AJAX Request : CodeDocHeader
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

$hCodeDoc = $hLib->CodeDoc;

/////////////////////////////////////////////////////////////////////////////////
// Build page

// Switch to SSL
$hSystem->EnsureSSL();

// Connect to database
$hSQL->ConnectTemporary();

// Get request parameters
$arrParameters = $hAJAX->GetRequestParameters( array( "CodeDocEvent", "CodeDocEventParam" ) );

$strCodeDocEvent = $arrParameters["CodeDocEvent"];
$strCodeDocEventParam = $arrParameters["CodeDocEventParam"];

// HTML output
$hHTML->DynamicTagStart();
	$hCodeDoc->_DisplayHeader( $strCodeDocEvent, $strCodeDocEventParam );
$hHTML->DynamicTagEnd();

// Disconnect from database
$hSQL->DisconnectTemporary();

?>