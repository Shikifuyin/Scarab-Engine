/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/JS/Lib/Debug.js
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Debugging tools
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
// Debugging interface
var _Debug_Div = null;

function _Debug_Enable()
{
	if ( _Debug_Div != null )
		return;

	document.write( "<div id='debugContent' style='display:block;"
					+ " position:absolute; top:7px; left:7px; padding:10px; width:300px;"
					+ " background:#0000ff; color:white; border:1px solid black;'></div>" );
	_Debug_Div = document.getElementById( "debugContent" );
}

function _Debug_Message( strMessage )
{
	if ( _Debug_Div == null )
		return;
	_Debug_Div.innerHTML += strMessage + "<br\/>";
}
function _Debug_Eval( strExpr )
{
	if ( _Debug_Div == null )
		return;
	_Debug_Message( strExpr + " = " + eval(strExpr) );
}
function _Debug_Inspect( hObject )
{
	if ( _Debug_Div == null )
		return;
	for( hProperty in hObject )
		_Debug_Message( hProperty );
}
function _Debug_Clear()
{
	if ( _Debug_Div == null )
		return;
	_Debug_Div.innerHTML = "";
}
