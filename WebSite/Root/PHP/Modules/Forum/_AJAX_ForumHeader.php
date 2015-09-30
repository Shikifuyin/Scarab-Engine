<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Forum/_AJAX_ForumHeader.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AJAX Request : ForumHeader
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

$hForum = $hLib->Forum;

/////////////////////////////////////////////////////////////////////////////////
// Build page

// Switch to SSL
$hSystem->EnsureSSL();

// Connect to database
$hSQL->ConnectTemporary();

// Get request parameters
$arrParameters = $hAJAX->GetRequestParameters( array( "ForumEvent", "ForumEventParam" ) );

$strForumEvent = $arrParameters["ForumEvent"];
$strForumEventParam = $arrParameters["ForumEventParam"];

// HTML output
$hHTML->DynamicTagStart();
	$hForum->_DisplayHeader( $strForumEvent, $strForumEventParam );
$hHTML->DynamicTagEnd();

// Disconnect from database
$hSQL->DisconnectTemporary();

?>