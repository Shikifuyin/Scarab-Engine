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
require_once "PHP/Master.php";

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Managers
$hLib = PHPLibrary::GetSingleton();

$hSystem = $hLib->System;

$hHTML = $hLib->HTML;
$hSQL = $hLib->SQL;
$hAJAX = $hLib->AJAX;

$hAccount = $hLib->Account;
$hSession = $hLib->Session;
$hForum = $hLib->Forum;
$hCodeDoc = $hLib->CodeDoc;
$hSearch = $hLib->Search;

/////////////////////////////////////////////////////////////////////////////////
// Build page

// Switch to SSL
$hSystem->EnsureSSL();

// Connect to database
$hSQL->ConnectTemporary();

// Check for a valid session
$bValidSession = $hSession->Open();

// Get request parameters
$arrParameters = $hAJAX->GetRequestParameters( array( "IndexEvent" ) );

$strIndexEvent = $arrParameters["IndexEvent"];

// HTML output
$hHTML->DynamicTagStart();
	// Footer animation
	switch( $strIndexEvent ) {
		case "ShowHome":
			/*$hHTML->SetID( "Home_Left" );
			$hHTML->SetStyleClass( "HomeContainer_Level0" );
			$hHTML->DivStart();
				$hHTML->SetID( "Home_Left_SplashScreen" );
				$hHTML->SetStyleClass( "HomeContainer_Level1" );
				$hHTML->DivStart();
					$hHTML->Image( "../Medias/Images/Button_Generic.png", "<BrokenImage>", 512, 512 );
				$hHTML->DivEnd();

			$hHTML->DivEnd();*/

			$hHTML->SetID( "Home_Right" );
			$hHTML->SetStyleClass( "HomeContainer_Level0 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "HomeContainer_Level2 HomeProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "Introduction" );
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "HomeContainer_Level2 HomeProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "Scarab-Engine is a game engine project aiming for professional-level features and performance." );
						$hHTML->Text( "It provides a fully unified and consistent API to game developpers that mixes simplicity, efficiency" );
						$hHTML->Text( "and functionality in an intuitive way." );
						$hHTML->LineBreak();
						$hHTML->Text( "Completely written from scratch, Scarab-Engine uses the fewest third party APIs as possible and" );
						$hHTML->Text( "ensures a platform-independant implementation." );
						$hHTML->LineBreak();
						$hHTML->Text( "This project is in constant development, currently in an educational state, check the forum for" );
						$hHTML->Text( "release patch-notes and future developments." );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "HomeContainer_Level2 HomeProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "Features" );
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "HomeContainer_Level2 HomeProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "- Stand-alone code written from scratch with native C++ and x86" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Extremely intuitive and easy to learn API and framework design" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Highly optimized code using top performance algorithms only" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Full-featured library including everything needed" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Resource management database and pipeline" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Rendering engine with high flexibility and advanced drawing tools" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Physics and collision detection engine handling animations" );
						$hHTML->LineBreak();
						$hHTML->Text( "- World scene management, LOD, visibility (portals and landscapes), ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "- Modeling management including primitives, curves, surfaces, extrusions, ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "- Graphical User Interface management using an event-driven, MVC framework" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Console environment, engine commands, scripts and configuration" );
						$hHTML->LineBreak();
						$hHTML->Text( "- Many more coming ... " );
					$hHTML->DivEnd();

				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "HomeContainer_Level2 HomeProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "Download" );
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "HomeContainer_Level2 HomeProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->SetID( "Download_LicenseCheckBox" );
						$hHTML->InputCheckBox( NULL, false );
						$hHTML->Text( "I have read and agree to the License and I accept the Terms & Conditions as detailed." );
					$hHTML->DivEnd();
	
					$hHTML->SetID( "Home_Right_Download_Link" );
					$hHTML->SetStyleClass( "HomeContainer_Level2" );
					$hHTML->DivStart();
						$hHTML->SetID( "Download_ConfirmButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"if ( document.getElementById('Download_LicenseCheckBox').checked )".
							"    window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=LastRelease' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

			$hHTML->DivEnd();
			break;
		case "ShowForum":
			$hForum->Display();
			break;
		case "ShowDemos":
			$hHTML->SetID( "Demos" );
			$hHTML->SetStyleClass( "DemosContainer_Level0 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "DemosContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "DemosContainer_Level2 DemosProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "Engine - Demos" );
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "DemosContainer_Level2 DemosProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->SetCustomAttribute( "frameborder", "0" );
						$hHTML->SetCustomAttribute( "allowfullscreen", "" );
						$hHTML->IFrameStart( "Frame_EngineDemos", 
										 	 "//www.youtube.com/embed/videoseries?list=PLy8fX9UgXhcuw4tUtvGaabfA7LtG4_3D7",
											 640, 480 ); // 369,277
						$hHTML->IFrameEnd();
						$hHTML->LineBreak();
						$hHTML->Text( "Here you will find fast samples of engine features ..." );
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "DemosContainer_Level2 DemosProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "Simple RPG - Demos" );
					$hHTML->DivEnd();
	
					$hHTML->SetStyleClass( "DemosContainer_Level2 DemosProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->SetCustomAttribute( "frameborder", "0" );
						$hHTML->SetCustomAttribute( "allowfullscreen", "" );
						$hHTML->IFrameStart( "Frame_SimpleRPGDemos", 
										 	 "//www.youtube.com/embed/videoseries?list=PLy8fX9UgXhctmVq8LrsvtpgzAXE1L1DUw",
											 640, 480 ); // 369,277
						$hHTML->IFrameEnd();
						$hHTML->LineBreak();
						$hHTML->Text( "The SimpleRPG demo will lead to the first real game from this engine." );
						$hHTML->LineBreak();
						$hHTML->Text( "The goal here will be to demonstrate (almost) all the engine capacities in an" );
						$hHTML->LineBreak();
						$hHTML->Text( "integrated game that will provide a classic RPG-like gameplay challenging the" );
						$hHTML->LineBreak();
						$hHTML->Text( "player with a set of solo challenges with increasing difficulty, up to pro-play." );
						$hHTML->LineBreak();
						$hHTML->Text( "Expect multi-phased fights, heavy multi-tasking, complex tanking and kitting techniques," );
						$hHTML->LineBreak();
						$hHTML->Text( "heal-sorting, character optimization, theory-crafting and much more ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "Please be patient while I continue coding ..." );
					$hHTML->DivEnd();

				$hHTML->DivEnd();

			$hHTML->DivEnd();
			break;
		case "ShowDocumentation":
			$hCodeDoc->Display();
			break;
		case "ShowAbout":
			$hHTML->SetID( "About" );
			$hHTML->SetStyleClass( "AboutContainer_Level0 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "AboutContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AboutContainer_Level2 AboutProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "About the Author" );
					$hHTML->DivEnd();
	
					$hHTML->SetStyleClass( "AboutContainer_Level2 AboutProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "Hello everyone !" );
						$hHTML->LineBreak();
						$hHTML->Text( "I am Remi Lefevre, a 28 year-old french developer and math geek like we say nowadays ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "I went to advanced math/physic and computer science courses in Paris, up to the master degree." );
						$hHTML->LineBreak();
						$hHTML->Text( "Although some early classes are really a must-have (Prep classes Math Sup/Spe), the last years" );
						$hHTML->LineBreak();
						$hHTML->Text( "were very disappointing for me with lack of technology independance and poor theory content." );
						$hHTML->LineBreak();
						$hHTML->Text( "Particularly true in the field of game development ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "In the last year of my cursus, I runned a research and development project on a realistic rendering" );
						$hHTML->LineBreak();
						$hHTML->Text( "technique mixing photon-mapping and radiosity approaches which was quite interesting." );
						$hHTML->LineBreak();
						$hHTML->Text( "Still, since I'm more of a self-taught person, I began coding at the age of 14 with C/C++ languages." );
						$hHTML->LineBreak();
						$hHTML->Text( "I'm a 'do-it-yourself' kind of a developper, I like to do things my way, with all details under my" );
						$hHTML->LineBreak();
						$hHTML->Text( "control, thus learning more about internal mechanics." );
						$hHTML->LineBreak();
						$hHTML->Text( "I am very picky with coding conventions and I am a refactoring maniac, you've been warned !" );
						$hHTML->LineBreak();
						$hHTML->Text( "Consequence of the above, I like to go dirty with low-level stuff and I focused myself on topics like" );
						$hHTML->LineBreak();
						$hHTML->Text( "x86 assembler, compilers, operating systems, network protocols, security, ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "Above all, I quickly found that game development is one of the most challenging and math/physics" );
						$hHTML->LineBreak();
						$hHTML->Text( "intensive topic. As a player myself, this was the way to go !" );
					$hHTML->DivEnd();

				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "AboutContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AboutContainer_Level2 AboutProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "About the Project" );
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "AboutContainer_Level2 AboutProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "Learning game development is not an easy task if, like me, you want to start from scratch. In fact, many" );
						$hHTML->LineBreak();
						$hHTML->Text( "free and open source engines are available but they almost allways have either bad code layout" );
						$hHTML->LineBreak();
						$hHTML->Text( "(barely readable and poorly commented) or bad framework/API approach leading to unintuitive usage and" );
						$hHTML->LineBreak();
						$hHTML->Text( "you often need lots of weird lines just to make sure the code runs in an optimal state ..." );
						$hHTML->LineBreak();
						$hHTML->Text( "Other ones are not full-fledge game engines and deal only with one aspect at a time ... You don't want" );
						$hHTML->LineBreak();
						$hHTML->Text( "to merge multiple big APIs like this !" );
						$hHTML->LineBreak();
						$hHTML->Text( "This is where Scarab-Engine comes into play, a project I started first as my own engine to write games," );
						$hHTML->LineBreak();
						$hHTML->Text( "but that ended up being a long-run learning tool too. The main goals are to produce a fully-featured" );
						$hHTML->LineBreak();
						$hHTML->Text( "and consistent game engine, that uses an intuitive API and framework, with high-end optimization both" );
						$hHTML->LineBreak();
						$hHTML->Text( "for algorithms and coding practices. It must(should) be multi-platform and portable." );
						$hHTML->LineBreak();
						$hHTML->Text( "Last but not least, the engine should provide flexibility to allow both quick setups and complete" );
						$hHTML->LineBreak();
						$hHTML->Text( "tweaking capabilities." );
					$hHTML->DivEnd();

				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "AboutContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "AboutContainer_Level2 AboutProperties_TitleText" );
					$hHTML->DivStart();
						$hHTML->Text( "Contact Me" );
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "AboutContainer_Level2 AboutProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "For any question, suggestion or complaint (or job offer !), feel free to contact me directly using the" );
						$hHTML->LineBreak();
						$hHTML->Text( "Scarab-Engine mailbox : " );
						$hHTML->SetStyleClass( "AboutProperties_EMailText" );
						$hHTML->SpanStart();
							$hHTML->Text( "scarab.engine@gmail.com" );
						$hHTML->SpanEnd();
					$hHTML->DivEnd();

				$hHTML->DivEnd();

			$hHTML->DivEnd();
			break;
		case "ShowReferences":
			$hHTML->SetID( "References" );
			$hHTML->SetStyleClass( "ReferencesContainer_Level0 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetStyleClass( "HomeContainer_Level1 ReferencesProperties_TitleText" );
				$hHTML->DivStart();
					$hHTML->Text( "Reference Books" );
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Game Engine Architecture (Jason Gregory)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A wonderful book to understand the structure of a game engine" );
						$hHTML->Text( "using a modular approach." );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Real-Time Collision Detection (Christer Ericson)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A good book to start approaching collision detection algorithms" );
						$hHTML->Text( "and get a grasp on the more complex algorithms." );
						$hHTML->Text( "Lacks depth on contact-point generation topic though ... but" );
						$hHTML->Text( "there's no real \"exhaustive\" source about this anyway ..." );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ReferencesProperties_TitleText" );
				$hHTML->DivStart();
					$hHTML->Text( "Reference Papers" );
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Efficient Large-Scale Sweep and  Prune Methods with AABB Insertion and Removal" );
						$hHTML->Text( "(Daniel J. Tracy, Samuel R. Buss, Bryan M. Woods)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "The only paper describing the wonderful segmented interval list implementation" );
						$hHTML->Text( "for the Sweep And Prune algorithm I use in my broad phase collision detection." );
						$hHTML->Text( "Implementation is quite tricky since there's a lot of sneaky cases that arise" );
						$hHTML->Text( "giving you some debugging headaches ..." );
						$hHTML->Text( "This is, of course, completely transparent in a Multi-SAP implementation." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "SAP_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=SAP' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "V-Clip: Fast and Robust Polyhedral Collision Detection (Brian Mirtich)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "The official paper for the Voronoi-Clip algorithm implemented in my narrow" );
						$hHTML->Text( "phase collision detection (GJK will join soon too)." );
						$hHTML->Text( "Some experience in computational geometry is required for proper implementation." );
						$hHTML->Text( "Contact generation is not described but it is quite easy to understand as you" );
						$hHTML->Text( "explicitely work with features ... (facet projection-intersection)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "VClip_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=VClip' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "A Fast and Robust GJK Implementation for Collision Detection of Convex Objects (Gino Van Den Bergen)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "The original Enhanced-GJK paper, all you want to know about GJK !" );
						$hHTML->Text( "Some knowledge in linear algebra / topology really helps here ..." );
						$hHTML->Text( "Again, contact generation is not described and it is a lot harder to figure out" );
						$hHTML->Text( "this time ... no feature data ! (facet sampling using normal perturbation and extremal queries)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "GinoGJK_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=GinoGJK' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "The Gilbert–Johnson–Keerthi Algorithm (Davor Jovanoski)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A more recent paper on GJK giving some improvements ... This one is a little" );
						$hHTML->Text( "more math-friendly and detailed but you can safely skip the demonstrations." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "BestGJK_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=BestGJK' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Real-time Collision Detection with Implicit Objects (Leif Olvang)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "An even more recent paper on GJK and it's extensions with great details and even some" );
						$hHTML->Text( "insight on contact point generation with explanations on the perturbed normal sampling");
						$hHTML->Text( "method ! Demystifies those Minkowski Portal Refinement and Expanding Polytope Algorithm ninjas ..." );
						$hHTML->Text( "If you got this far you should be able to implement your own GJK-EPA ! Still, making it" );
						$hHTML->Text( "both numerically and geometrically stable is hard. You've been warned !" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "MPREPA_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=MPREPA' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Stable, Robust, and Versatile Multibody Dynamics Animation (Kenny Erleben)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "The best paper I've been able to find that describes a physic engine's" );
						$hHTML->Text( "constraint solving theory in a math-consistent way ... Beware though, you" );
						$hHTML->Text( "should be familiar with (numerical) integration, differential systems," );
						$hHTML->Text( "linear algebra, combinatory optimisation, and Newton/Lagrange physics (Yeah !)." );
						$hHTML->Text( "Also describes stacking levels and shock-propagation." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "Erleben_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=Erleben' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Iterative Dynamics with Temporal Coherence (Erin Catto)" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A simpler and more intuitive, but less formal, approach to a physic engine's" );
						$hHTML->Text( "constraint solving theory ... Much less prerequisites here and you can still" );
						$hHTML->Text( "manage to get some stacking to work !" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "CattoID_DownloadButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( '" . $hSystem->GetQueryHostRoot() . "/Download/Download.php?file=CattoID' );"								
						);
						$hHTML->InputButton( "Download" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "HomeContainer_Level1 ReferencesProperties_TitleText" );
				$hHTML->DivStart();
					$hHTML->Text( "Reference Web Sources" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Geometric Tools" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A great source of math code, particularly for algebra/analysis." );
						$hHTML->Text( "Also a well-designed rendering/modeling/scene system I took inspiration from." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "GeometricTools_LinkButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( 'http://www.geometrictools.com/index.html' );"								
						);
						$hHTML->InputButton( "Open" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Bullet Physics" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A great source of computational geometry/collision detection/physics code," );
						$hHTML->Text( "which inspired me for the constraint solver design in the physics manager." );
						$hHTML->Text( "Their implementation of GJK-EPA is great but their code is overall hard to read" );
						$hHTML->Text( "and I haven't got my hands in it yet." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "BulletPhysics_LinkButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( 'http://bulletphysics.org/wordpress/' );"								
						);
						$hHTML->InputButton( "Open" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Box 2D" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "A great source of well-documented code to begin understanding physics engines," );
						$hHTML->Text( "which inspired me for the island solver design in the physics manager." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "Box2D_LinkButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( 'http://box2d.org/about/' );"								
						);
						$hHTML->InputButton( "Open" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Game Dev" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "THE Game Development forum and resource center ... what else ?" );
						$hHTML->Text( "A huge source for troubleshooting or finding precise case-specific information." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "GameDev_LinkButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( 'http://www.gamedev.net/page/index.html' );"								
						);
						$hHTML->InputButton( "Open" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "HomeContainer_Level1 ClearFix" );
				$hHTML->DivStart();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_HeaderText" );
					$hHTML->DivStart();
						$hHTML->Text( "Speculative Contacts" );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_ContentText" );
					$hHTML->DivStart();
						$hHTML->Text( "The unique resource I found that explains speculative contacts generation." );
						$hHTML->Text( "Easy to implement with VClip, a little harder with GJK ..." );
						$hHTML->Text( "Basically it gives you almost as much power as continuous collision detection" );
						$hHTML->Text( "without ever touching any TOI computation, in a discrete collision engine," );
						$hHTML->Text( "and you get contact caching advantages for free on the way !" );
						$hHTML->Text( "All of this for a very cheap computational price, much less than CCD." );
					$hHTML->DivEnd();
					$hHTML->SetStyleClass( "ReferencesContainer_Level2 ReferencesProperties_DownloadButton" );
					$hHTML->DivStart();
						$hHTML->SetID( "SpeculativeContacts_LinkButton" );
						$hHTML->SetEventCallback (
							HTML_EVENT_ONMOUSECLICK,
							"window.open( 'http://www.wildbunny.co.uk/blog/2011/03/25/".
								"speculative-contacts-an-continuous-collision-engine-approach-part-1/' );"								
						);
						$hHTML->InputButton( "Open" );
					$hHTML->DivEnd();
				$hHTML->DivEnd();

			$hHTML->DivEnd();
			break;
		case "ShowCredits":
			$hHTML->SetID( "Credits" );
			$hHTML->SetStyleClass( "CreditsContainer_Level0 ClearFix" );
			$hHTML->DivStart();
				$hHTML->SetID( "Credits_Title" );
				$hHTML->SetStyleClass( "CreditsContainer_Level1" );
				$hHTML->DivStart();
					$hHTML->Text( "The " );
					$hHTML->SetStyleInline( "color:#ee8a86" );
					$hHTML->SpanStart();
						$hHTML->Text( "SCARAB" );
					$hHTML->SpanEnd();
					$hHTML->Text( "-" );
					$hHTML->SetStyleInline( "color:#ee8122" );
					$hHTML->SpanStart();
						$hHTML->Text( "Engine" );
					$hHTML->SpanEnd();
					$hHTML->Text( " Project" );
				$hHTML->DivEnd();

				$hHTML->SetID( "Credits_SubTitle" );
				$hHTML->SetStyleClass( "CreditsContainer_Level1" );
				$hHTML->DivStart();
					$hHTML->TextRaw( "A <span style=\"color:#ee8a86\">S</span>imple yet <span style=\"color:#ee8a86\">C</span>omplete ".
								  	 "<span style=\"color:#ee8a86\">A</span>rchitecture for <span style=\"color:#ee8a86\">R</span>endering, ".
								  	 "<span style=\"color:#ee8a86\">A</span>nimating and <span style=\"color:#ee8a86\">B</span>uilding ".
								  	 "interactive worlds." );
				$hHTML->DivEnd();

				$hHTML->SetID( "Credits_Author" );
				$hHTML->SetStyleClass( "CreditsContainer_Level1" );
				$hHTML->DivStart();
					$hHTML->Text( "By Remi \"Shikifuyin\" Lefevre" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

				$hHTML->SetID( "Credits_License" );
				$hHTML->SetStyleClass( "CreditsContainer_Level1" );
				$hHTML->DivStart();
					$hHTML->Text( "This project is licensed under the" );
					$hHTML->LineBreak();
					$hHTML->HyperLinkStart( "http://creativecommons.org/licenses/by-nc-nd/3.0/", NULL, HTML_TARGET_BLANK, "license" );
						$hHTML->Text( "Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License" );
					$hHTML->HyperLinkEnd();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SectionText" );
				$hHTML->DivStart();
					$hHTML->Text( "Third Party Module" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Low-Level Assembly (x86)" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Low-Level System API and Modules" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Win32/GDI Support API" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "DirectX 9 Support API" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SectionText" );
				$hHTML->DivStart();
					$hHTML->Text( "Library Module" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "String Manipulation" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
				
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Error Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Memory Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Data-Structures Implementation" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Object Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Object : SmartPointers & Garbage Collector" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Object : Reflection Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Math Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Math : Algebra Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Math : Analysis Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Math : Geometry Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Math : Hash/Random Library" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
				
				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SectionText" );
				$hHTML->DivStart();
					$hHTML->Text( "Engine Module" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Resource Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Resource : RDB Database Design and Implementation" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Resource : Assets pipeline" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

                    $hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Resource : XML & COLLADA support" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Rendering Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Rendering : Geometry & Textures" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Rendering : Shaders & Effects" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Modeling Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Modeling : Curves & Surfaces" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Modeling : LOD" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Physics Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Physics : Island Solver" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Physics : Collision Detection" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Physics : Kinematics Simulation" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Physics : Rigid-Body Dynamics" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Physics : Soft-Body Dynamics" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "World Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "World : Scene Hierarchy" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "World : Landscapes & Portals" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "World : Culling, Casting and PathFinding" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Input Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Input : Action Mapping" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "GUI Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "GUI : MVC Architecture" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "GUI : Overlays & Widgets" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "GUI : Tools & Effects" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Console Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Console : Commands" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Console : Environment & Scripting" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Application Manager" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SectionText" );
				$hHTML->DivStart();
					$hHTML->Text( "Web Site" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "PHP/SQL/AJAX Engine" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "PHP : XML Module" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "PHP : HTML 5 Module" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "PHP : CSS 3 Module" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Session Engine" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Forum Engine" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Code Documentation Engine" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Interface Conception" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Laurent \"Lolo2M\" Serris" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Graphical Design" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Laurent \"Lolo2M\" Serris" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Integration Hell" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "Remi \"Shikifuyin\" Lefevre" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SectionText" );
				$hHTML->DivStart();
					$hHTML->Text( "Special Thanks" );
				$hHTML->DivEnd();

				$hHTML->SetStyleClass( "CreditsContainer_Level1 CreditsProperties_SeparatorLine" );
				$hHTML->DivStart();
					$hHTML->HorizontalLine();
				$hHTML->DivEnd();
				
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "My Mom !" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "For supporting me during countless coding hours ..." );
							$hHTML->LineBreak();
							$hHTML->Text( "Supporting my endless speeches about stuff she doesn't care about ..." );
							$hHTML->LineBreak();
							$hHTML->Text( "... and giving me food, bed and clean clothes !" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();
					
					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "My Friends !" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "I've been playing the mole in my room(cave) for a very long time, that" );
							$hHTML->LineBreak();
							$hHTML->Text( "takes dedication you know ... Even though they kept inviting me" );
							$hHTML->LineBreak();
							$hHTML->Text( "and it saved me from becomming some gollum-like coding-freak !" );
							$hHTML->LineBreak();
							$hHTML->Text( "A special word here for my best-friend Lolo2M who made the \"artistic\"" );
							$hHTML->LineBreak();
							$hHTML->Text( "part of the website : Took a while, no ? At last Dude !" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "My Teachers !" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "A special word here about two teachers that really made me love" );
							$hHTML->LineBreak();
							$hHTML->Text( "maths and physics even more than I already did back in the days ..." );
							$hHTML->LineBreak();
							$hHTML->Text( "I won't be as proficient as I am today if it weren't for them." );
							$hHTML->LineBreak();
							$hHTML->Text( "My math professor, Mr Bonnot, from mid-school who took extra hours to" );
							$hHTML->LineBreak();
							$hHTML->Text( "teach me extra maths about stuff I asked him (traditional greek problems)." );
							$hHTML->LineBreak();
							$hHTML->Text( "My physics professor, Mrs Vivicorsi, from preparatory class who walked me" );
							$hHTML->LineBreak();
							$hHTML->Text( "from pure maths to theoric and applied physics." );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Internet !" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "For being such an immense source of information without which I would" );
							$hHTML->LineBreak();
							$hHTML->Text( "never had been able to learn from so much papers, documentations" );
							$hHTML->LineBreak();
							$hHTML->Text( "and open-source code ..." );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

					$hHTML->SetStyleClass( "CreditsContainer_Level1 ClearFix" );
					$hHTML->DivStart();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_HeaderText" );
						$hHTML->DivStart();
							$hHTML->Text( "Some Book ??!" );
						$hHTML->DivEnd();
						$hHTML->SetStyleClass( "CreditsContainer_Level2 CreditsProperties_ContentText" );
						$hHTML->DivStart();
							$hHTML->Text( "That I read one day, I think it was about 3D rendering & animation" );
							$hHTML->LineBreak();
							$hHTML->Text( "algorithms, and there was a note in the introduction saying something" );
							$hHTML->LineBreak();
							$hHTML->Text( "like \"Ok, now maybe you already have it in mind, the best game ever," );
							$hHTML->LineBreak();
							$hHTML->Text( "mixing RPG, FPS and STR in a MMO, built upon your own full-featured" );
							$hHTML->LineBreak();
							$hHTML->Text( "engine specially crafted and ... Well, That's just not gonna happen !" );
							$hHTML->LineBreak();
							$hHTML->Text( "Such an engine takes whole teams of developpers and countless hours ...\"" );
							$hHTML->LineBreak();
							$hHTML->Text( "At this time, some subtle mechanism triggered in my mind and I though ..." );
							$hHTML->LineBreak();
							$hHTML->Text( "CHALLENGE ACCEPTED !" );
						$hHTML->DivEnd();
					$hHTML->DivEnd();

			$hHTML->DivEnd();
			break;
		case "ShowRegisterPage":
			$hAccount->Display( ACCOUNT_DISPLAY_REGISTRATION );
			break;
		case "ShowAccountPage":
			$hAccount->Display( ACCOUNT_DISPLAY_MANAGEMENT );
			break;
		default:
			$arrSearchEvent = explode( "##", $strIndexEvent );
			assert( 'count($arrSearchEvent) == 2' );
			assert( '$arrSearchEvent[0] == "ShowSearchResults"' );
			$hSearch->Query( $arrSearchEvent[1] );
			$hSearch->Display();
			break;
	}
$hHTML->DynamicTagEnd();

// Close session
if ( $bValidSession )
	$hSession->Close();

// Disconnect from database
$hSQL->DisconnectTemporary();

?>