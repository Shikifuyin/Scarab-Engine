/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/MyLandscape.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Testing stuff ...
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
#include "MyLandscape.h"

/////////////////////////////////////////////////////////////////////////////////
// MyLandscape implementation
//MyLandscape::MyLandscape( VertexFormat * pVF, EffectInstance * pEffect ):
//    WorldOutdoorArea( Vertex2::Null, Vector2(4.0f,4.0f), 50, 50,
//                      16, 16, 4, TEXT("MyLandscape") )
//{
//    m_pVF = pVF;
//    m_pEffect = pEffect;
//
//    UShort * arrHeightsA = m_arrHeightFieldA;
//    UShort * arrHeightsB = m_arrHeightFieldB;
//
//    Scalar fStep = ( SCALAR_2PI / 15.0f );
//    Scalar fT, fU, fSin, fCos, fZ;
//    UShort iZ;
//
//    fU = 0.0f;
//    for( UInt iY = 0; iY < 16; ++iY ) {
//        fSin = ( (MathFn->Sin(fU) + 1.0f) * 0.5f );
//        fT = 0.0f;
//        for( UInt iX = 0; iX < 16; ++iX ) {
//            fCos = ( (MathFn->Cos(fT) + 1.0f) * 0.5f );
//            fZ = ( (fSin * fCos) * (Scalar)USHORT_MAX );
//            iZ = (UShort)( MathFn->Round(fZ) );
//
//            *arrHeightsA++ = iZ;
//            *arrHeightsB++ = USHORT_MAX - iZ;
//
//            fT += fStep;
//        }
//        fU += fStep;
//    }
//}
//MyLandscape::~MyLandscape()
//{
//    DestroyRegions();
//}
//
//Void MyLandscape::_OnRegionLoad( UInt iRegionX, UInt iRegionY, WorldOutdoorRegion * pRegion,
//                                 const Vertex2 & vOrigin, const Vector2 & vSize ) const
//{
//    UShort * arrHeightField = pRegion->GetHeightField();
//
//    if ( (iRegionX & 1) == (iRegionY & 1) )
//        MemCopy( arrHeightField, m_arrHeightFieldA, 256 * sizeof(UShort) );
//    else
//        MemCopy( arrHeightField, m_arrHeightFieldB, 256 * sizeof(UShort) );
//
//    pRegion->Finalize( vOrigin, vSize, -0.5f, +0.5f, m_pVF, m_pEffect );
//}
//Void MyLandscape::_OnRegionUnload( UInt iRegionX, UInt iRegionY, WorldOutdoorRegion * pRegion ) const
//{
//    // nothing to do
//}

