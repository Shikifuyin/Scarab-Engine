/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/Menu.js
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Menu animations (dynamic styles using Javascript)
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

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

/////////////////////////////////////////////////////////////////////////////////
// Menu events
function UpdateMenuBeforeContent( strTarget )
{
	// Animate content
	hContentAnimation.Target = 0.0;
	hContentAnimation.Run();
	
	// Animate menu
	hMenuAnimation.Target = strTarget;
	hMenuAnimation.Run();
}
function UpdateMenuAfterContent( bAnimateContent )
{
	// Animate chain : footer then content
	hFooterAnimation.AnimateContent = bAnimateContent;
	hFooterAnimation.Run();
}

/////////////////////////////////////////////////////////////////////////////////
// Menu animation
function _MenuAnimation_Begin( hAnimation )
{
	var hMenuSelector = document.getElementById( "Header_Menu_Selector" );
	
	// Setup for linear animation
	if ( hMenuSelector.style.left == "" )
		hMenuSelector.style.left = "0%";
	hAnimation.Current = parseInt( hMenuSelector.style.left, 10 );
	switch( hAnimation.Target ) {
		case "Home":          hAnimation.End = 0; break;
		case "Forum":         hAnimation.End = 20; break;
		case "Demos":         hAnimation.End = 40; break;
		case "Documentation": hAnimation.End = 60; break;
		case "About":         hAnimation.End = 80; break;
		default: break; // Never happen
	}
	hAnimation.Inc = ( hAnimation.End - hAnimation.Current ) / ( hAnimation.iFrameCount - 1 );
}
function _MenuAnimation_End( hAnimation )
{
	hAnimation.Current = 0;
    hAnimation.End = 0;
	hAnimation.Inc = 0;
}
function _MenuAnimation_Update( hAnimation )
{
	var hMenuSelector = document.getElementById( "Header_Menu_Selector" );

	// Step animation
	hAnimation.Current += hAnimation.Inc;
	if ( hAnimation.Inc > 0 && hAnimation.Current > hAnimation.End )
		hAnimation.Current = hAnimation.End;
	else if ( hAnimation.Inc < 0 && hAnimation.Current < hAnimation.End )
		hAnimation.Current = hAnimation.End;

	hMenuSelector.style.left = ( hAnimation.Current.toString() + "%" );
}

var hMenuAnimation = new Animation( 100, 500, 500, _MenuAnimation_Begin, _MenuAnimation_End, _MenuAnimation_Update );


/////////////////////////////////////////////////////////////////////////////////
// Content animation
function _ContentAnimation_Begin( hAnimation )
{
	var hContent = document.getElementById( "Content" );

	// Setup for linear animation
	hAnimation.Current = parseFloat( hContent.style.opacity );
	hAnimation.End = hAnimation.Target;
	hAnimation.Inc = ( hAnimation.End - hAnimation.Current ) / ( hAnimation.iFrameCount - 1 );
}
function _ContentAnimation_End( hAnimation )
{
	hAnimation.Current = 0.0;
    hAnimation.End = 0.0;
	hAnimation.Inc = 0.0;
}
function _ContentAnimation_Update( hAnimation )
{
	var hContent = document.getElementById( "Content" );

	// Step animation
	hAnimation.Current += hAnimation.Inc;
	if ( hAnimation.Inc > 0 && hAnimation.Current > hAnimation.End )
		hAnimation.Current = hAnimation.End;
	else if ( hAnimation.Inc < 0 && hAnimation.Current < hAnimation.End )
		hAnimation.Current = hAnimation.End;

	hContent.style.opacity = hAnimation.Current.toString();
}

var hContentAnimation = new Animation( 100, 500, 0, _ContentAnimation_Begin, _ContentAnimation_End, _ContentAnimation_Update );

/////////////////////////////////////////////////////////////////////////////////
// Footer animation
function _FooterAnimation_Begin( hAnimation )
{
	var hHeader = document.getElementById( "Header" );
	var hContent = document.getElementById( "Content" );
	var hFooter = document.getElementById( "Footer" );

	var minTop = ( window.innerHeight - hFooter.offsetHeight );

	// Setup for linear animation
	hAnimation.Current = parseInt( hFooter.style.top, 10 );
	hAnimation.End = ( hHeader.offsetHeight + hContent.offsetHeight );
	if ( hAnimation.End < minTop )
	    hAnimation.End = minTop;
	hAnimation.Inc = ( hAnimation.End - hAnimation.Current ) / ( hAnimation.iFrameCount - 1 );
}
function _FooterAnimation_End( hAnimation )
{
	hAnimation.Current = 0;
    hAnimation.End = 0;
	hAnimation.Inc = 0;
	
	if ( hAnimation.AnimateContent ) {
		hContentAnimation.Target = 1.0;
		hContentAnimation.Run();
		hAnimation.AnimateContent = false;
	}
}
function _FooterAnimation_Update( hAnimation )
{
	var hFooter = document.getElementById( "Footer" );

	// Step animation
	hAnimation.Current += hAnimation.Inc;
	if ( hAnimation.Inc > 0 && hAnimation.Current > hAnimation.End )
		hAnimation.Current = hAnimation.End;
	else if ( hAnimation.Inc < 0 && hAnimation.Current < hAnimation.End )
		hAnimation.Current = hAnimation.End;

	hFooter.style.top = ( hAnimation.Current.toString() + "px" );
}

var hFooterAnimation = new Animation( 100, 500, 0, _FooterAnimation_Begin, _FooterAnimation_End, _FooterAnimation_Update );


