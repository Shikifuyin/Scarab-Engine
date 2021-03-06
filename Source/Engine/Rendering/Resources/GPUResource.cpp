/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Rendering/Resources/GPUResource.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : GPU Resource : Common interface
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
#include "GPUResource.h"

/////////////////////////////////////////////////////////////////////////////////
// GPUResource implementation
GPUResource::GPUResource()
{
    m_pDeviceResource = NULL;
}
GPUResource::~GPUResource()
{
    // nothing to do
}

