/////////////////////////////////////////////////////////////////////////////////
// File : <Root>/JS/Lib/Animation.js
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0
// Began Code : 05/06/2012
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Animation tools
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
// Animation interface
function Animation( iFrameCount, fDuration, fDelay, pfOnBegin, pfOnEnd, pfOnUpdate )
{
	// Members
	this.iFrameCount = iFrameCount;
	this.fDuration = fDuration;
	this.fDelay = fDelay;
	
	this.pfOnBegin = pfOnBegin;
	this.pfOnEnd = pfOnEnd;
	this.pfOnUpdate = pfOnUpdate;
	
	this.fTimeStep = ( fDuration / iFrameCount );

	this.iFrame = 0;
	this.fTime = 0.0; // Normalized to [0;1]
	this.iDelayID = 0;
	this.iIntervalID = 0;

	var hThisObject = this;
	
	// Internal Methods
	this._IntervalCallback = function() {
		// Check for animation end
		if ( hThisObject.iFrame >= hThisObject.iFrameCount || hThisObject.fTime >= hThisObject.fDuration ) {
			clearInterval( hThisObject.iIntervalID );
			hThisObject.pfOnEnd( hThisObject );
			hThisObject.iIntervalID = 0;
			return;
		}

		// Step animation
		++(hThisObject.iFrame);
		hThisObject.fTime += hThisObject.fTimeStep;

		hThisObject.pfOnUpdate( hThisObject );
	}
	
	// Methods
	this.Run = function() {
		// Interrupt current run if any
		if ( hThisObject.iDelayID != 0) {
			clearTimeout( hThisObject.iDelayID );
			hThisObject.iDelayID = 0;
		} else if ( hThisObject.iIntervalID != 0 ) {
			clearInterval( hThisObject.iIntervalID );
			hThisObject.pfOnEnd( hThisObject );
			hThisObject.iIntervalID = 0;
		}

		// Start animation
		hThisObject.iFrame = 0;
		hThisObject.fTime = 0.0;

		if ( hThisObject.fDelay == 0.0 ) {
			hThisObject.pfOnBegin( hThisObject );
			hThisObject.iIntervalID = setInterval( hThisObject._IntervalCallback, hThisObject.fTimeStep );
		} else {
			hThisObject.iDelayID = setTimeout( function() {
					hThisObject.iDelayID = 0;
					hThisObject.pfOnBegin( hThisObject );
					hThisObject.iIntervalID = setInterval( hThisObject._IntervalCallback, hThisObject.fTimeStep );
				}, hThisObject.fDelay
			);
		}
	}
	this.Stop = function() {
		// Interrupt current run if any
		if ( hThisObject.iDelayID != 0) {
			clearTimeout( hThisObject.iDelayID );
			hThisObject.iDelayID = 0;
		} else if ( hThisObject.iIntervalID != 0 ) {
			clearInterval( hThisObject.iIntervalID );
			hThisObject.pfOnEnd( hThisObject );
			hThisObject.iIntervalID = 0;
		}
	}
}


