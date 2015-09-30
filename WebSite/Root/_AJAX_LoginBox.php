<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/_AJAX_LoginBox.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : AJAX Request : LoginBox
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : TODO = Update Content upon connect/disconnect
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
require_once "PHP/Master.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Managers
$hLib = PHPLibrary::GetSingleton();

$hSystem = $hLib->System;

$hHTML = $hLib->HTML;
$hSQL = $hLib->SQL;
$hAJAX = $hLib->AJAX;

$hUser = $hLib->User;
$hSession = $hLib->Session;

// Helpers
function DisplayFormAuthentify()
{
	global $hHTML;
	global $hAJAX;

	$hHTML->SetStyleClass( "LoginBoxContainer_Level0" );
	$hHTML->DivStart();
		$hHTML->SetID( "LoginBox_Login" );
		$hHTML->InputText();
	$hHTML->DivEnd();

	$hHTML->SetStyleClass( "LoginBoxContainer_Level0" );
	$hHTML->DivStart();
		$hHTML->SetID( "LoginBox_Password" );
		$hHTML->InputPassword();
	$hHTML->DivEnd();

	$hHTML->SetStyleClass( "LoginBoxContainer_Level0 LoginBoxProperties_Button" );
	$hHTML->DivStart();
		$hHTML->SetID( "LoginBox_Register" );
		$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowRegisterPage'" ),
							 "UpdateMenuBeforeContent('Home');", NULL, 500  );
		$hHTML->InputButton( "Register" );
	$hHTML->DivEnd();
	
	$hHTML->SetStyleClass( "LoginBoxContainer_Level0 LoginBoxProperties_Button" );
	$hHTML->DivStart();
		$hHTML->SetID( "LoginBox_Connect" );
		$hAJAX->SetEventTags (
			HTML_EVENT_ONMOUSECLICK, array( "Header_LoginBox", "Content" ),
			array( array( "document.getElementById('LoginBox_Login').value",
						  "document.getElementById('LoginBox_Password').value" ),
				   array( "'ShowHome'" ) ),
			"UpdateMenuBeforeContent('Home');", NULL, 500
		);
		$hHTML->InputButton( "Connect" );
	$hHTML->DivEnd();
}
function DisplayFormWelcome()
{
	global $hHTML;
	global $hAJAX;
	global $hUser;
	global $hSession;

	$strUserID = $hSession->GetUserID();
	$arrUserInfos = $hUser->GetInfosByID( $strUserID );
	$strUserLogin = $arrUserInfos["Login"];

	$hHTML->SetStyleClass( "LoginBoxContainer_Level0 LoginBoxProperties_HelloText" );
	$hHTML->DivStart();
		$hHTML->Text( "Welcome, $strUserLogin" );
	$hHTML->DivEnd();
	
	$hHTML->SetStyleClass( "LoginBoxContainer_Level0 LoginBoxProperties_Button" );
	$hHTML->DivStart();
		$hHTML->SetID( "LoginBox_Account" );
		$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowAccountPage'" ),
					 	 	 "UpdateMenuBeforeContent('Home');", NULL, 500  );
		$hHTML->InputButton( "Account" );
	$hHTML->DivEnd();

	$hHTML->SetStyleClass( "LoginBoxContainer_Level0 LoginBoxProperties_Button" );
	$hHTML->DivStart();
		$hHTML->SetID( "LoginBox_Disconnect" );
		$hAJAX->SetEventTags(
			HTML_EVENT_ONMOUSECLICK, array( "Header_LoginBox", "Content" ),
			array( array( "'Disconnect'", "''" ), array( "'ShowHome'" ) ),
			"UpdateMenuBeforeContent('Home');", NULL, 500
		);
		$hHTML->InputButton( "Disconnect" );
	$hHTML->DivEnd();
}

/////////////////////////////////////////////////////////////////////////////////
// Build page

// Switch to SSL
$hSystem->EnsureSSL();

// Connect to database
$hSQL->ConnectTemporary();

// Check for a valid session
$bValidSession = $hSession->Open();

// Get request parameters
$arrParameters = $hAJAX->GetRequestParameters( array( "ParamLogin", "ParamPassword" ) );

$strParamLogin = $arrParameters["ParamLogin"];
$strParamPassword = $arrParameters["ParamPassword"];

// HTML output
$hHTML->DynamicTagStart();
	if ( $bValidSession ) {
		if ( $strParamLogin == "Disconnect" ) {
			$hSession->Destroy();

			DisplayFormAuthentify();
		} else {
			DisplayFormWelcome();
			$hSession->Close();
		}
	} else {
		if ( $strParamLogin != "" && $strParamPassword != "" ) {
			$strUserID = $hUser->Authentify( $strParamLogin, $strParamPassword );
			if ( $strUserID != NULL ) {
				$bOk = $hSession->Create( $strUserID );
				assert( '$bOk' );

				DisplayFormWelcome();
				$hSession->Close();
			} else
				DisplayFormAuthentify();
		} else
			DisplayFormAuthentify();
	}
$hHTML->DynamicTagEnd();

// Close session
if ( $bValidSession )
	$hSession->Close();

// Disconnect from database
$hSQL->DisconnectTemporary();

?>