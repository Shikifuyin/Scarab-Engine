<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/_AJAX_Content.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AJAX Request : Content
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
require_once "../PHP/Master.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Managers
$hLib = PHPLibrary::GetSingleton();

$hSystem = $hLib->System;

/////////////////////////////////////////////////////////////////////////////////
// Download

// Switch to SSL
$hSystem->EnsureSSL();

// Get requested file
assert( '$hSystem->HasVariableGET( "file" )' );
$strFile = $hSystem->GetVariableGET( "file" );

$strFileName = "";
switch( $strFile ) {
	// Releases
	case "LastRelease": $strFileName = "LastRelease/Scarab-Engine.v1.0.r004.rar"; break;

	// References
	case "SAP": 	$strFileName = "References/SAP_paper.pdf"; break;
	case "VClip": 	$strFileName = "References/TR97-05.pdf"; break;
	case "GinoGJK": $strFileName = "References/EnhancedGJK.pdf"; break;
	case "BestGJK": $strFileName = "References/BestGJK.pdf"; break;
	case "MPREPA":  $strFileName = "References/MPR and EPA.pdf"; break;
	case "Erleben": $strFileName = "References/erleben.050401.pdf"; break;
	case "CattoID": $strFileName = "References/IterativeDynamics.pdf"; break;

	default: assert( 'false' ); break;
}

// Send file
$hSystem->HeaderStartDownload( $strFileName );

?>