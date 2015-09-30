/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUIContainer.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Empty Container
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
#include "GUIContainer.h"

/////////////////////////////////////////////////////////////////////////////////
// GUIContainer implementation
GUIContainer::GUIContainer( GUIWidget * pParent, const GUILayout & guiLayout, const GChar * strName ):
    GUIWidget( NULL, pParent, guiLayout, strName )
{
    // nothing to do
}
GUIContainer::~GUIContainer()
{
    // nothing to do
}
