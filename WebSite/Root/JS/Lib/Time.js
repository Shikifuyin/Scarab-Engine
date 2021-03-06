/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/JS/Lib/Time.js
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Timing tools
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
// Timing interface
function Wait( iMilliseconds )
{
	var dateStart = new Date();
	var dateCur = new Date();
	while( (dateCur - dateStart) < iMilliseconds )
		dateCur = new Date();
}


