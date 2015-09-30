<?php
/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/index.php
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Entry point
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
$hAccount = $hLib->Account;
$hSession = $hLib->Session;
$hForum = $hLib->Forum;
$hCodeDoc = $hLib->CodeDoc;
$hSearch = $hLib->Search;

// Index events
define( "INDEX_EVENT_SHOW_HOME",          "ShowHome" );
define( "INDEX_EVENT_SHOW_FORUM",         "ShowForum" );
define( "INDEX_EVENT_SHOW_DEMOS",         "ShowDemos" );
define( "INDEX_EVENT_SHOW_DOCUMENTATION", "ShowDocumentation" );
define( "INDEX_EVENT_SHOW_ABOUT",         "ShowAbout" );
define( "INDEX_EVENT_SHOW_REFERENCES",    "ShowReferences" );
define( "INDEX_EVENT_SHOW_CREDITS",       "ShowCredits" );
define( "INDEX_EVENT_SHOW_REGISTER",      "ShowRegisterPage" );
define( "INDEX_EVENT_SHOW_ACCOUNT",       "ShowAccountPage" );

/////////////////////////////////////////////////////////////////////////////////
// Build page

// Platform checks
//$bOK = $hSystem->CheckPHPVersionString( "5.4.4" );
//assert( '$bOK' );

// Switch to SSL
$hSystem->EnsureSSL();

// Connect to database
$hSQL->ConnectTemporary();

// Check for account activation
if ( $hSystem->HasVariableGET( "activate" ) ) {
	$strActivationSeed = $hSystem->GetVariableGET( "activate" );
	$hUser->Activate( $strActivationSeed );

	// Disconnect from database
	$hSQL->DisconnectTemporary();

	// Redirect
	$hSystem->HeaderRedirect( $hSystem->GetQueryURL() );
}

// Check for a valid session
$bValidSession = $hSession->Open();

// HTML output
$hHTML->DocumentStart();
	$hHTML->HeadStart();
		// Title
		$hHTML->HeadTitle( "Scarab Engine - WebSite" );

		// Meta data
		$hHTML->HeadMetaName( "Scarab Engine" );
		$hHTML->HeadMetaAuthor( "Remi \"Shikifuyin\" Lefevre" );
		$hHTML->HeadMetaDescription( "Game Engine Project" );
		$hHTML->HeadMetaCopyright( "Scarab Engine, 2013" );
		$hHTML->HeadMetaLanguage( "en" );

		$hHTML->HeadMetaRobot( "INDEX,FOLLOW" );
		$hHTML->HeadMetaKeywords( "Game,Engine,Scarab,Remi,Lefevre,Shiki,Shikifuyin" );
		$hHTML->HeadMetaKeywords( "3D,Simulation,Rendering,Modeling,Animation,Physics,Dynamics,Kinematics" );
		$hHTML->HeadMetaKeywords( "C,C++,Assembler,Asm,x86,DirectX,DX,OpenGL" );

		// Linked files
		//$hHTML->HeadLink( "http://fonts.googleapis.com/css?family=Fjalla+One", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "http://fonts.googleapis.com/css?family=Open+Sans", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );

		$hHTML->HeadLink( "CSS/Global.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/Index.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/Home.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/Forum.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/Demos.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/CodeDoc.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/About.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/Account.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/References.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );
		$hHTML->HeadLink( "CSS/Credits.css", MIME_TEXT_CSS, HTML_RELATION_STYLESHEET );

		$hHTML->HeadScriptExternal( "JS/Lib/Debug.js", MIME_TEXT_JAVASCRIPT );
		$hHTML->HeadScriptExternal( "JS/Lib/Time.js", MIME_TEXT_JAVASCRIPT );
		$hHTML->HeadScriptExternal( "JS/Lib/Animation.js", MIME_TEXT_JAVASCRIPT );
		$hHTML->HeadScriptExternal( "Menu.js", MIME_TEXT_JAVASCRIPT );

		// AJAX
		$hAJAX->Initialize();
		$hAJAX->GenerateTagRequest( "Header_LoginBox", "_AJAX_LoginBox.php", array( "ParamLogin", "ParamPassword" ) );
		$hAJAX->GenerateTagRequest( "Content", "_AJAX_Content.php", array( "IndexEvent" ), "UpdateMenuAfterContent(true);" );
		$hAJAX->UpdateTag( false, "Header_LoginBox", array( "''", "''" ) );
		$hAJAX->UpdateTag( false, "Content", array( "'ShowHome'" ) );
		$hAccount->InitializeTagRequests();
		$hForum->InitializeTagRequests();
		$hCodeDoc->InitializeTagRequests();
		$hSearch->InitializeTagRequests();

		// Facebook API support
		$hHTML->HeadScriptStart( MIME_TEXT_JAVASCRIPT );
		$hHTML->HeadScriptCode( "function _FacebookInit(d, s, id) {" );
		$hHTML->HeadScriptCode( "    var js, fjs = d.getElementsByTagName(s)[0];" );
		$hHTML->HeadScriptCode( "    if ( d.getElementById(id) )" );
		$hHTML->HeadScriptCode( "        return;" );
		$hHTML->HeadScriptCode( "    js = d.createElement(s);" );
		$hHTML->HeadScriptCode( "    js.id = id;" );
		$hHTML->HeadScriptCode( "    js.src = '//connect.facebook.net/en_US/all.js#xfbml=1';" );
		$hHTML->HeadScriptCode( "    fjs.parentNode.insertBefore(js, fjs);" );
		$hHTML->HeadScriptCode( "}" );
		$hHTML->HeadScriptCode( "_FacebookInit( document, 'script', 'facebook-jssdk' );" );
		$hHTML->HeadScriptEnd();

		// Twitter API support
		$hHTML->HeadScriptStart( MIME_TEXT_JAVASCRIPT );
		$hHTML->HeadScriptCode( "function _TwitterInit(d, s, id) {" );
		$hHTML->HeadScriptCode( "    var js, fjs = d.getElementsByTagName(s)[0];" );
		$hHTML->HeadScriptCode( "    if ( d.getElementById(id) )" );
		$hHTML->HeadScriptCode( "        return;" );
		$hHTML->HeadScriptCode( "    js = d.createElement(s);" );
		$hHTML->HeadScriptCode( "    js.id = id;" );
		$hHTML->HeadScriptCode( "    js.src = '//platform.twitter.com/widgets.js';" );
		$hHTML->HeadScriptCode( "    fjs.parentNode.insertBefore(js, fjs);" );
		$hHTML->HeadScriptCode( "}" );
		$hHTML->HeadScriptCode( "_TwitterInit( document, 'script', 'twitter-wjs' );" );
		$hHTML->HeadScriptEnd();

	$hHTML->HeadEnd();

	$hHTML->BodyStart();
		// Header
		$hHTML->SetID( "Header" );
		$hHTML->SetStyleClass( "ClearFix" );
		$hHTML->HeaderStart();
			// Left Container
			$hHTML->SetID( "Header_Left" );
			$hHTML->SetStyleClass( "HeaderContainer_Level0 HeaderProperties_LeftContainer ClearFix" );
			$hHTML->DivStart();
				// Search box
				/*$hHTML->SetID( "Header_SearchBox" );
				$hHTML->SetStyleClass( "ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "SearchBoxContainer_Level0 SearchBoxProperties_InputText" );
					$hHTML->DivStart();
						$hHTML->SetID( "Search_QueryString" );
						$hHTML->InputText();
					$hHTML->DivEnd();
	
					$hHTML->SetStyleClass( "SearchBoxContainer_Level0 SearchBoxProperties_SearchButton" );
					$hHTML->DivStart();
						$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content",
											 array( "'ShowSearchResults##'+document.getElementById('Search_QueryString').value" ),
											 "UpdateMenuBeforeContent('Home');", NULL, 500  );
						$hHTML->InputButton( "Search" );
					$hHTML->DivEnd();
	
				$hHTML->DivEnd();*/

			$hHTML->DivEnd();

			// Center Container
			$hHTML->SetID( "Header_Center" );
			$hHTML->SetStyleClass( "HeaderContainer_Level0 HeaderProperties_CenterContainer ClearFix" );
			$hHTML->DivStart();
				// Menu
				$hHTML->SetID( "Header_Menu" );
				$hHTML->SetStyleClass( "ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetID( "Header_Menu_HomeButton" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Button" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowHome'" ),
										 "UpdateMenuBeforeContent('Home');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "Home" );
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Header_Menu_Separator_HF" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Separator" );
					$hHTML->DivStart();
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Header_Menu_ForumButton" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Button" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowForum'" ),
										 "UpdateMenuBeforeContent('Forum');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "Forum" );
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Header_Menu_Separator_FD" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Separator" );
					$hHTML->DivStart();
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Header_Menu_DemosButton" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Button" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowDemos'" ),
										 "UpdateMenuBeforeContent('Demos');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "Demos" );
					$hHTML->DivEnd();
					
					$hHTML->SetID( "Header_Menu_Separator_DD" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Separator" );
					$hHTML->DivStart();
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Header_Menu_DocumentationButton" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Button" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowDocumentation'" ),
										 "UpdateMenuBeforeContent('Documentation');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "Documentation" );
					$hHTML->DivEnd();
					
					$hHTML->SetID( "Header_Menu_Separator_DC" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Separator" );
					$hHTML->DivStart();
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Header_Menu_AboutButton" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Button" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowAbout'" ),
										 "UpdateMenuBeforeContent('About');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "About" );
					$hHTML->DivEnd();

					$hHTML->SetID( "Header_Menu_Selector" );
					$hHTML->SetStyleClass( "MenuContainer_Level0 MenuProperties_Selector" );
					$hHTML->SetStyleInline( "left:0%;" ); // Init for animation
					$hHTML->DivStart();
					$hHTML->DivEnd();
				$hHTML->DivEnd();

			$hHTML->DivEnd();

			// Right Container
			$hHTML->SetID( "Header_Right" );
			$hHTML->SetStyleClass( "HeaderContainer_Level0 HeaderProperties_RightContainer ClearFix" );
			$hHTML->DivStart();
				// Login box
				$hHTML->SetID( "Header_LoginBox" );
				$hHTML->SetStyleClass( "ClearFix" );
				$hHTML->DivStart();
					// AJAX Dynamic Div
				$hHTML->DivEnd();

			$hHTML->DivEnd();

		$hHTML->HeaderEnd();

		// Content
		$hHTML->SetID( "Content" );
		$hHTML->SetStyleClass( "ClearFix" );
		$hHTML->SetStyleInline( "opacity:1.0;" ); // Init for animation
		$hHTML->DivStart();
			// AJAX Dynamic Div
		$hHTML->DivEnd();

		// Footer
		$hHTML->SetID( "Footer" );
		$hHTML->SetStyleClass( "ClearFix" );
		$hHTML->SetStyleInline( "top:1000px;" ); // Init for animation
		$hHTML->FooterStart();
				// Social networks
				$hHTML->SetID( "Footer_SocialNetworks" );
				$hHTML->SetStyleClass( "FooterContainer_Level0 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetID( "Footer_SocialNetwork_Facebook" );
					$hHTML->SetStyleClass( "SocialNetworkContainer_Level0 SocialNetworkProperties_Facebook" );
					$hHTML->DivStart();
						$hHTML->SetID( "fb-root" );
						$hHTML->DivStart();
							// Nothing here ...
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "fb-like" );
						$hHTML->SetCustomAttribute( "data-href",       $hSystem->GetQueryHostRoot() );
						$hHTML->SetCustomAttribute( "data-layout",     "button_count" );
						$hHTML->SetCustomAttribute( "data-width",      "64" );
						$hHTML->SetCustomAttribute( "data-font",       "arial" );
						$hHTML->SetCustomAttribute( "data-send",       "false" );
						$hHTML->SetCustomAttribute( "data-show-faces", "false" );
						$hHTML->DivStart();
							// Nothing here ...
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetID( "Footer_SocialNetwork_Twitter" );
					$hHTML->SetStyleClass( "SocialNetworkContainer_Level0 SocialNetworkProperties_Twitter" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "twitter-share-button" );
						$hHTML->HyperLinkStart( "https://twitter.com/share" );
							$hHTML->Text( "Tweet" );
						$hHTML->HyperLinkEnd();
					$hHTML->DivEnd();
				
				$hHTML->DivEnd();
				
				// License
				$hHTML->SetID( "Footer_License" );
				$hHTML->SetStyleClass( "FooterContainer_Level0 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetID( "Footer_License_Icon" );
					$hHTML->DivStart();
						$hHTML->HyperLinkStart( "http://creativecommons.org/licenses/by-nc-nd/3.0/", NULL, HTML_TARGET_BLANK, "license" );
							$hHTML->SetStyleInline( "border-width:0;" );
							$hHTML->Image( "http://i.creativecommons.org/l/by-nc-nd/3.0/88x31.png", "Licence Creative Commons" );
						$hHTML->HyperlinkEnd();
					$hHTML->DivEnd();

					$hHTML->SetID( "Footer_License_Text" );
					$hHTML->DivStart();
						$hHTML->SetCustomAttribute( "xmlns:dct", "http://purl.org/dc/terms/" );
						$hHTML->SetCustomAttribute( "property", "dct:title" );
						$hHTML->SpanStart();
							$hHTML->Text( "Scarab-Engine" );
						$hHTML->SpanEnd();
						$hHTML->Text( " is licensed under the " );
						$hHTML->HyperLinkStart( "http://creativecommons.org/licenses/by-nc-nd/3.0/", NULL, HTML_TARGET_BLANK, "license" );
							$hHTML->Text( "Creative Commons" );
							$hHTML->LineBreak();
							$hHTML->Text( "Attribution-NonCommercial-NoDerivs 3.0 Unported License" );
						$hHTML->HyperlinkEnd();
					$hHTML->DivEnd();

				$hHTML->DivEnd();

				// Links
				$hHTML->SetID( "Footer_Links" );
				$hHTML->SetStyleClass( "FooterContainer_Level0 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetID( "Footer_Link_References" );
					$hHTML->SetStyleClass( "LinkContainer_Level0 LinkProperties_Link" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowReferences'" ),
									 	 "UpdateMenuBeforeContent('Home');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "References" );
					$hHTML->DivEnd();

					$hHTML->SetID( "Footer_LinkSeparator_RC" );
					$hHTML->SetStyleClass( "LinkContainer_Level0 LinkProperties_Separator" );
					$hHTML->DivStart();
					$hHTML->DivEnd();

					$hHTML->SetID( "Footer_Link_Credits" );
					$hHTML->SetStyleClass( "LinkContainer_Level0 LinkProperties_Link" );
					$hAJAX->SetEventTag( HTML_EVENT_ONMOUSECLICK, "Content", array( "'ShowCredits'" ),
									 	 "UpdateMenuBeforeContent('Home');", NULL, 500 );
					$hHTML->DivStart();
						$hHTML->Text( "Credits" );
					$hHTML->DivEnd();

				$hHTML->DivEnd();

		$hHTML->FooterEnd();

	$hHTML->BodyEnd();

$hHTML->DocumentEnd();

// Close session
if ( $bValidSession )
	$hSession->Close();

// Disconnect from database
$hSQL->DisconnectTemporary();

?>