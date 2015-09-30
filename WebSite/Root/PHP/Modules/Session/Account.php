<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/PHP/Modules/Session/Account.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Modules, Session
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

	// Account events
define( "ACCOUNT_DISPLAY_REGISTRATION",    "DisplayRegistration" );
define( "ACCOUNT_DISPLAY_REGISTERED",      "DisplayRegistered" );

define( "ACCOUNT_EVENT_REGISTER", "AccountRegister" );

define( "ACCOUNT_DISPLAY_MANAGEMENT", "DisplayManagement" );

define( "ACCOUNT_EVENT_MANAGE_UPDATE", "AccountManageUpdate" );

/////////////////////////////////////////////////////////////////////////////////
// The AccountManager class
class AccountManager
{
    // Singleton interface
    public static function GetSingleton() {
        if ( AccountManager::$m_hInstance == NULL )
            AccountManager::$m_hInstance = new AccountManager();
        return AccountManager::$m_hInstance;
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
	
	// Main routines
	public function InitializeTagRequests()
	{
		$hAJAX = AJAXManager::GetSingleton();

		$hAJAX->GenerateTagRequest( "AccountContent", "PHP/Modules/Session/_AJAX_AccountContent.php", array( "AccountEvent", "AccountEventParam" ),
																								      "UpdateMenuAfterContent(false);" );
		$hAJAX->UpdateTag( false, "AccountContent", array( "'DisplayRegistration'", "''" ) );
	}

	public function Display( $strEvent )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "AccountContent" );
		$hHTML->SetStyleClass( "AccountContainer_Level0 ClearFix" );
		$hHTML->DivStart();
			$this->_DisplayContent( $strEvent, "" );
		$hHTML->DivEnd();
	}

	// Internal routines (don't call yourself !)
	public function _DisplayContent( $strEvent, $strEventParam )
	{
		// Update from event
		$this->_Update( $strEvent, $strEventParam );

		// Display response
		switch( $strEvent ) {
			case ACCOUNT_DISPLAY_REGISTRATION:
				$strMessage = NULL;
				if ( $strEventParam != "" )
					$strMessage = $strEventParam;
				
				$this->_DisplayRegistration( $strMessage );
				break;
			case ACCOUNT_DISPLAY_REGISTERED:
				$strMessage = NULL;
				if ( $strEventParam != "" )
					$strMessage = $strEventParam;
				
				$this->_DisplayRegistered( $strMessage );
				break;

			case ACCOUNT_DISPLAY_MANAGEMENT:
				$strMessage = NULL;
				if ( $strEventParam != "" )
					$strMessage = $strEventParam;

				$this->_DisplayManagement( $strMessage );
				break;

			default: assert( 'false' ); break;
		}
	}

	// Helpers
		// Update routines
	private function _Update( & $strEvent, & $strEventParam )
	{
		$hSystem = SystemManager::GetSingleton();
		$hUser = UserManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();
		
		$bValidSession = $hSession->Open();

		switch( $strEvent ) {
			case ACCOUNT_EVENT_REGISTER:
				$arrParameters = explode( "##", $strEventParam );
				$strLogin = $arrParameters[0];
				$strPassword0 = $arrParameters[1];
				$strPassword1 = $arrParameters[2];
				$strEMail = $arrParameters[3];
				$strFirstName = $arrParameters[4];
				$strLastName = $arrParameters[5];
				$iBirthdayYear = intval( $arrParameters[6] );
				$iBirthdayMonth = intval( $arrParameters[7] );
				$iBirthdayDay = intval( $arrParameters[8] );
				$iGender = intval( $arrParameters[9] );
				$strCountry = $arrParameters[10];
				$strAvatar = $arrParameters[11];

				// Check parameters
				if ( $strLogin == "" ) {
					$strEvent = ACCOUNT_DISPLAY_REGISTRATION;
					$strEventParam = "Invalid/Missing required field : Login";
					return;
				}
				if ( $strPassword0 == "" || $strPassword1 == "" ) {
					$strEvent = ACCOUNT_DISPLAY_REGISTRATION;
					$strEventParam = "Invalid/Missing required field : Password";
					return;
				}
				if ( $strPassword0 != $strPassword1 ) {
					$strEvent = ACCOUNT_DISPLAY_REGISTRATION;
					$strEventParam = "Passwords do not match !";
					return;
				}
				if ( $strEMail == "" ) {
					$strEvent = ACCOUNT_DISPLAY_REGISTRATION;
					$strEventParam = "Invalid/Missing required field : E-Mail";
					return;
				}

				// Generate activation seed
				$strActivationSeed = $hSystem->Random( 32, true );

				// Register unactivated account
				$iUserID = $hUser->Create( $strActivationSeed,
										   $strLogin, $strPassword0, $strEMail, $strFirstName, $strLastName,
										   date( "Y-m-d", mktime(0,0,0,$iBirthdayMonth,$iBirthdayDay,$iBirthdayYear) ),
										   $iGender, $strCountry, $strAvatar );
				if ( $iUserID == NULL ) {
					$strEvent = ACCOUNT_DISPLAY_REGISTRATION;
					$strEventParam = "Login and/or E-Mail already used !";
					return;
				}

				// Send activation e-mail
				$strLink = $hSystem->GetQueryHostRoot() . "/index.php?activate=$strActivationSeed";

				$hSystem->EMail( $strEMail, "Welcome to Scarab-Engine ! (Account activation)",
					"Thank you for registering on the Scarab-Engine website !\r\n".
					"To verify this e-mail address and activate your account, please follow this link :\r\n".
					"\r\n".
					"\t$strLink\r\n".
					"\r\n".
					"Feel free to come and share your feedback in the forum !\r\n".
					"",
					"scarab.engine@gmail.com",
					"Reply-To: scarab.engine@gmail.com" . "\r\n"
				);

				// Success
				$strEvent = ACCOUNT_DISPLAY_REGISTERED;
				$strEventParam = $strEMail;
				break;

			case ACCOUNT_EVENT_MANAGE_UPDATE:
				assert( '$bValidSession' );

				$arrParameters = explode( "##", $strEventParam );
				$strPassword0 = $arrParameters[0];
				$strPassword1 = $arrParameters[1];
				$strEMail = $arrParameters[2];
				$strFirstName = $arrParameters[3];
				$strLastName = $arrParameters[4];
				$iBirthdayYear = intval( $arrParameters[5] );
				$iBirthdayMonth = intval( $arrParameters[6] );
				$iBirthdayDay = intval( $arrParameters[7] );
				$iGender = intval( $arrParameters[8] );
				$strCountry = $arrParameters[9];
				$strAvatar = $arrParameters[10];

				$iUserID = $hSession->GetUserID();
				$arrUserInfos = $hUser->GetInfosByID( $iUserID );

				// Update password
				if ( $strPassword0 != "" || $strPassword1 != "" ) {
					if ( $strPassword0 != $strPassword1 ) {
						$strEvent = ACCOUNT_DISPLAY_MANAGEMENT;
						$strEventParam = "Passwords do not match !";
						return;
					}
					$hUser->UpdatePassword( $iUserID, $strPassword0 );
				}

				// Update email
				if ( $strEMail != "" && $strEMail != $arrUserInfos["EMail"] ) {
					$bOK = $hUser->UpdateEMail( $iUserID, $strEMail );
					if ( !$bOK ) {
						$strEvent = ACCOUNT_DISPLAY_MANAGEMENT;
						$strEventParam = "E-Mail already used !";
						return;
					}
				}

				// Update name
				$hUser->UpdateName( $iUserID, $strFirstName, $strLastName );

				// Update infos
				$hUser->UpdateInfos( $iUserID, date( "Y-m-d", mktime(0,0,0,$iBirthdayMonth,$iBirthdayDay,$iBirthdayYear) ),
											   $iGender, $strCountry );

				// Update avatar
				$hUser->UpdateAvatar( $iUserID, $strAvatar );

				// Success
				$strEvent = ACCOUNT_DISPLAY_MANAGEMENT;
				$strEventParam = "Updated account data successfully !";
				break;
			default:
				// nothing to do
				break;
		}

		if ( $bValidSession )
			$hSession->Close();
	}

		// Display routines : Registration
	private function _DisplayRegistration( $strMessage = NULL )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();

		$hHTML->SetID( "AccountContent_Registration_Header" );
		$hHTML->SetStyleClass( "AccountContainer_Level1 ClearFix" );
		$hHTML->DivStart();
			if ( $strMessage != NULL ) {
				$hHTML->SetID( "AccountContent_Registration_Header_Message" );
				$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_AccountHeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( $strMessage );
				$hHTML->DivEnd();
			}
		$hHTML->DivEnd();

		$hHTML->SetID( "AccountContent_Registration_Form" );
		$hHTML->SetStyleClass( "AccountContainer_Level1 ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Login" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_Login" );
					$hHTML->InputText();
					
					$hHTML->SetStyleInline( "color:#ff0000;" );
					$hHTML->SpanStart();	
						$hHTML->Text( "*" );
					$hHTML->SpanEnd();
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Password" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_Password0" );
					$hHTML->InputPassword();
					
					$hHTML->SetStyleInline( "color:#ff0000;" );
					$hHTML->SpanStart();	
						$hHTML->Text( "*" );
					$hHTML->SpanEnd();
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Confirm Password" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_Password1" );
					$hHTML->InputPassword();
					
					$hHTML->SetStyleInline( "color:#ff0000;" );
					$hHTML->SpanStart();	
						$hHTML->Text( "*" );
					$hHTML->SpanEnd();
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "E-Mail" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_EMail" );
					$hHTML->InputEMail();
					
					$hHTML->SetStyleInline( "color:#ff0000;" );
					$hHTML->SpanStart();	
						$hHTML->Text( "*" );
					$hHTML->SpanEnd();
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "First Name" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_FirstName" );
					$hHTML->InputText();
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Last Name" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_LastName" );
					$hHTML->InputText();
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Birthday" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountRegistration_Birthday_Year" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Year", "unselected" );
							for( $i = 2010; $i >= 1910; --$i )
								$hHTML->ComboListItem( "$i", "$i" );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountRegistration_Birthday_Month" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Month", "unselected" );
							for( $i = 1; $i <= 12; ++$i )
								$hHTML->ComboListItem( "$i", "$i" );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountRegistration_Birthday_Day" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Day", "unselected" );
							for( $i = 1; $i <= 31; ++$i )
								$hHTML->ComboListItem( "$i", "$i" );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();

				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Gender" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountRegistration_Gender" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Woman", "0", true );
							$hHTML->ComboListItem( "Man", "1" );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleInline( "color:#ff0000;" );
					$hHTML->SpanStart();	
						$hHTML->Text( "*" );
					$hHTML->SpanEnd();
				$hHTML->DivEnd();	
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Country" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_Country" );
					$hHTML->InputText();
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Avatar" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountRegistration_Avatar" );
					$hHTML->InputURL();
				$hHTML->DivEnd();
			$hHTML->DivEnd();

		$hHTML->DivEnd();

		$hHTML->SetID( "AccountContent_Registration_Footer" );
		$hHTML->SetStyleClass( "AccountContainer_Level1 ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "AccountContent_Registration_Footer_PrivacyPolicy" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_FooterLeftItem" );
			$hHTML->DivStart();
				$hHTML->Text( "By creating an account, you accept the Terms and Conditions of the Privacy Policy." );
			$hHTML->DivEnd();

			$hHTML->SetID( "AccountContent_Registration_Footer_Cancel" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_FooterRightItem" );
			$hHTML->DivStart();
				$hHTML->SetID( "AccountRegistration_CancelButton" );
				$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowHome'" ),
									 "UpdateMenuBeforeContent('Home');", NULL, 500 );
				$hHTML->InputButton( "Cancel" );
			$hHTML->DivEnd();

			$hHTML->SetID( "AccountContent_Registration_Footer_Confirm" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_FooterRightItem" );
			$hHTML->DivStart();
				$hHTML->SetID( "AccountRegistration_ConfirmButton" );
				$hAJAX->SetEventTag (
					HTML_EVENT_ONMOUSECLICK, "AccountContent",
					array( "'AccountRegister'", "document.getElementById('AccountRegistration_Login').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_Password0').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_Password1').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_EMail').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_FirstName').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_LastName').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_Birthday_Year').value + '##' +".
												"document.getElementById('AccountRegistration_Birthday_Month').value + '##' +".
												"document.getElementById('AccountRegistration_Birthday_Day').value + '##' +".
												"document.getElementById('AccountRegistration_Gender').value + '##' +".
												"document.getElementById('AccountRegistration_Country').value.replace(/&/g,'%26') + '##' +".
												"document.getElementById('AccountRegistration_Avatar').value.replace(/&/g,'%26')" )
				);
				$hHTML->InputButton( "Confirm" );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
	}
	private function _DisplayRegistered( $strEMail )
	{
		$hHTML = HTMLManager::GetSingleton();
		
		$hHTML->SetID( "AccountContent_Registered" );
		$hHTML->SetStyleClass( "AccountContainer_Level1" );
		$hHTML->DivStart();
			$hHTML->Text( "Registration Successfull ! An e-mail has been sent to $strEMail ..." );
			$hHTML->LineBreak();
			$hHTML->Text( "Use this mail's embedded link to activate your account !" );
		$hHTML->DivEnd();
	}

		// Display routines : Management
	private function _DisplayManagement( $strMessage = NULL )
	{
		$hHTML = HTMLManager::GetSingleton();
		$hAJAX = AJAXManager::GetSingleton();
		$hUser = UserManager::GetSingleton();
		$hSession = SessionManager::GetSingleton();
		
		$bValidSession = $hSession->Open();
		if ( !$bValidSession )
			return;

		$iUserID = $hSession->GetUserID();
		$arrUserInfos = $hUser->GetInfosByID( $iUserID );
		$strLogin = $arrUserInfos["Login"];
		$strEMail = $arrUserInfos["EMail"];
		$strFirstName = $arrUserInfos["FirstName"];
		$strLastName = $arrUserInfos["LastName"];
		$arrBirthday = explode( "-", $arrUserInfos["Birthday"] );
		$iGender = $arrUserInfos["Gender"];
		$strCountry = $arrUserInfos["Country"];
		$strAvatar = $arrUserInfos["Avatar"];
		$strRank = $arrUserInfos["Rank"];
		$iMessageCount = $arrUserInfos["MessageCount"];

		$hHTML->SetID( "AccountContent_Management_Header" );
		$hHTML->SetStyleClass( "AccountContainer_Level1 ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "AccountContent_Management_Header_Banner" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_AccountHeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Account management panel for $strLogin" );
			$hHTML->DivEnd();
			
			$hHTML->SetID( "AccountContent_Management_Header_Ranking" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_AccountHeaderText" );
			$hHTML->DivStart();
				$hHTML->Text( "Achieved Rank : " );
				$hHTML->SetStyleInline( "color:#ee8a86;" );
				$hHTML->SpanStart();
					$hHTML->Text( $strRank );
				$hHTML->SpanEnd();
				$hHTML->Text( " ($iMessageCount Messages) !" );
			$hHTML->DivEnd();

			if ( $strMessage != NULL ) {
				$hHTML->SetID( "AccountContent_Management_Header_Message" );
				$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_AccountHeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( $strMessage );
				$hHTML->DivEnd();
			}
		$hHTML->DivEnd();

		$hHTML->SetID( "AccountContent_Management_Form" );
		$hHTML->SetStyleClass( "AccountContainer_Level1 ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Password" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_Password0" );
					$hHTML->InputPassword();
				$hHTML->DivEnd();	
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Confirm Password" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_Password1" );
					$hHTML->InputPassword();
				$hHTML->DivEnd();	
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "E-Mail" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_EMail" );
					$hHTML->InputEMail( $strEMail );
				$hHTML->DivEnd();
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "First Name" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_FirstName" );
					$hHTML->InputText( $strFirstName );
				$hHTML->DivEnd();	
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Last Name" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_LastName" );
					$hHTML->InputText( $strLastName );
				$hHTML->DivEnd();	
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Birthday" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountManagement_Birthday_Year" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Year", "unselected" );
							for( $i = 2010; $i >= 1910; --$i ) {
								$hHTML->ComboListItem( "$i", "$i", ($i == $arrBirthday[0]) );
							}
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountManagement_Birthday_Month" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Month", "unselected" );
							for( $i = 1; $i <= 12; ++$i )
								$hHTML->ComboListItem( "$i", "$i", ($i == $arrBirthday[1]) );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountManagement_Birthday_Day" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Day", "unselected" );
							for( $i = 1; $i <= 31; ++$i )
								$hHTML->ComboListItem( "$i", "$i", ($i == $arrBirthday[2]) );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();

				$hHTML->DivEnd();	
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Gender" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AccountProperties_ItemSelect SelectStyler" );
					$hHTML->DivStart();
						$hHTML->SetID( "AccountManagement_Gender" );
						$hHTML->InputComboListStart();
							$hHTML->ComboListItem( "Woman", "0", ($iGender == 0) );
							$hHTML->ComboListItem( "Man", "1", ($iGender == 1) );
						$hHTML->InputComboListEnd();
					$hHTML->DivEnd();
				$hHTML->DivEnd();
			$hHTML->DivEnd();

			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Country" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_Country" );
					$hHTML->InputText( $strCountry );
				$hHTML->DivEnd();	
			$hHTML->DivEnd();
			
			$hHTML->SetStyleClass( "AccountContainer_Level2 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_HeaderText" );
				$hHTML->DivStart();
					$hHTML->Text( "Avatar" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AccountContainer_Level3 AccountProperties_ContentText" );
				$hHTML->DivStart();
					$hHTML->SetID( "AccountManagement_Avatar" );
					$hHTML->InputURL();
				$hHTML->DivEnd();	
			$hHTML->DivEnd();

		$hHTML->DivEnd();

		$hHTML->SetID( "AccountContent_Management_Footer" );
		$hHTML->SetStyleClass( "AccountContainer_Level1 ClearFix" );
		$hHTML->DivStart();
			$hHTML->SetID( "AccountContent_Management_Footer_Cancel" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_FooterRightItem" );
			$hHTML->DivStart();
				$hHTML->SetID( "AccountManagement_CancelButton" );
				$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowHome'" ),
									 "UpdateMenuBeforeContent('Home');", NULL, 500 );
				$hHTML->InputButton( "Cancel" );
			$hHTML->DivEnd();

			$hHTML->SetID( "AccountContent_Management_Footer_Confirm" );
			$hHTML->SetStyleClass( "AccountContainer_Level2 AccountProperties_FooterRightItem" );
			$hHTML->DivStart();
				$hHTML->SetID( "AccountManagement_ConfirmButton" );
				$hAJAX->SetEventTag (
					HTML_EVENT_ONMOUSECLICK, "AccountContent",
					array( "'AccountManageUpdate'", "document.getElementById('AccountManagement_Password0').value.replace(/&/g,'%26') + '##' +".
													"document.getElementById('AccountManagement_Password1').value.replace(/&/g,'%26') + '##' +".
													"document.getElementById('AccountManagement_EMail').value.replace(/&/g,'%26') + '##' +".
													"document.getElementById('AccountManagement_FirstName').value.replace(/&/g,'%26') + '##' +".
													"document.getElementById('AccountManagement_LastName').value.replace(/&/g,'%26') + '##' +".
													"document.getElementById('AccountManagement_Birthday_Year').value + '##' +".
													"document.getElementById('AccountManagement_Birthday_Month').value + '##' +".
													"document.getElementById('AccountManagement_Birthday_Day').value + '##' +".
													"document.getElementById('AccountManagement_Gender').value + '##' +".
													"document.getElementById('AccountManagement_Country').value.replace(/&/g,'%26') + '##' +".
													"document.getElementById('AccountManagement_Avatar').value.replace(/&/g,'%26')" )
				);
				$hHTML->InputButton( "Confirm" );
			$hHTML->DivEnd();

		$hHTML->DivEnd();
		
		$hSession->Close();
	}

    // Members
}
?>