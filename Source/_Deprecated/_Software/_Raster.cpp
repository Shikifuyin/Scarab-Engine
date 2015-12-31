/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Graphics/3D/Raster.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Delegate of the Renderer, handles rasterization process
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-Noncommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "Raster.h"
#include "Renderer.h"

/////////////////////////////////////////////////////////////////////////////////
// Raster implementation
Raster::Raster( Renderer * pRenderer )
{
    m_pRenderer = pRenderer;
    m_pCFL = NULL;
    m_ScanConv.pPixels[0] = NULL;
    m_ScanConv.pPixels[1] = NULL;
    m_ScanConv.pSlopePixels[0] = NULL;
    m_ScanConv.pSlopePixels[1] = NULL;
    m_ScanLine.pPixels[0] = NULL;
    m_ScanLine.pPixels[1] = NULL;
    m_ScanLine.pSlope = NULL;
    m_ScanLine.pCurrent = NULL;
}
Raster::~Raster()
{
    if (m_pCFL != NULL) {
        m_pCFL->Free(m_ScanConv.pPixels[0]);
        m_pCFL->Free(m_ScanConv.pPixels[1]);
        m_pCFL->Free(m_ScanConv.pSlopePixels[0]);
        m_pCFL->Free(m_ScanConv.pSlopePixels[1]);
        m_pCFL->Free(m_ScanLine.pPixels[0]);
        m_pCFL->Free(m_ScanLine.pPixels[1]);
        m_pCFL->Free(m_ScanLine.pSlope);
        m_pCFL->Free(m_ScanLine.pCurrent);
    }
}

Void Raster::SetCFL( CustomFragmentLayout * pCFL )
{
    if (m_pCFL != NULL) {
        m_pCFL->Free(m_ScanConv.pPixels[0]);
        m_pCFL->Free(m_ScanConv.pPixels[1]);
        m_pCFL->Free(m_ScanConv.pSlopePixels[0]);
        m_pCFL->Free(m_ScanConv.pSlopePixels[1]);
        m_pCFL->Free(m_ScanLine.pPixels[0]);
        m_pCFL->Free(m_ScanLine.pPixels[1]);
        m_pCFL->Free(m_ScanLine.pSlope);
        m_pCFL->Free(m_ScanLine.pCurrent);
    }

    m_pCFL = pCFL;

    if (m_pCFL != NULL) {
        m_ScanConv.pPixels[0] = m_pCFL->Allocate();
        m_ScanConv.pPixels[1] = m_pCFL->Allocate();
        m_ScanConv.pSlopePixels[0] = m_pCFL->Allocate();
        m_ScanConv.pSlopePixels[1] = m_pCFL->Allocate();
        m_ScanLine.pPixels[0] = m_pCFL->Allocate();
        m_ScanLine.pPixels[1] = m_pCFL->Allocate();
        m_ScanLine.pSlope = m_pCFL->Allocate();
        m_ScanLine.pCurrent = m_pCFL->Allocate();
    }
}

Void Raster::ScanPoint( CCFLPtr pPixelA )
{
    // IMPORTANT : Fields of pixels A B and C are already scaled by InvW !
    // See _VertexToPixel() in the Renderer class ...
}
Void Raster::ScanSegment( CCFLPtr pPixelA, CCFLPtr pPixelB )
{
    // IMPORTANT : Fields of pixels A B and C are already scaled by InvW !
    // See _VertexToPixel() in the Renderer class ...

    const Scalar *pP0, *pP1;
    const Vertex2 * pSwapPos;
    const Vertex2 * pPos0 = (const Vertex2 *)( m_pCFL->GetField( pPixelA, CFLSEMANTIC_RASTERPOS, 0 ) );
    const Vertex2 * pPos1 = (const Vertex2 *)( m_pCFL->GetField( pPixelB, CFLSEMANTIC_RASTERPOS, 0 ) );

    // Sort pointers by crescent Y
    Scalar fDY = (pPos1->Y - pPos0->Y);
    if (fDY < 0.0f) {
        fDY = -fDY;
        pP0 = pPixelB;
        pP1 = pPixelA;
        pSwapPos = pPos0;
        pPos0 = pPos1;
        pPos1 = pSwapPos;
    } else {
        pP0 = pPixelA;
        pP1 = pPixelB;
    }
    Scalar fDX = (pPos1->X - pPos0->X);
    if (fDY >= fDX) {
        if ( fDY < SCALAR_ERROR )
            return; // degenerate segment
        Scalar fInvDY = MathFn->Invert(fDY);

        // Top-left filling-convention, top part
		Int startY = MathFn->Ceil( pPos0->Y );
		Int endY = MathFn->Ceil( pPos1->Y ) - 1;
        if ( endY < startY )
            return; // degenerate segment
        Scalar fOfsY = ( (Scalar)startY - pPos0->Y );

        // Slopes
        Scalar fSlopeX = fDX * fInvDY;
        Scalar fCurX = pPos0->X + fSlopeX * fOfsY;
        UInt j;
        for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
            m_ScanLine.pSlope[j] = ( pP1[j] - pP0[j] ) * fInvDY;
            m_ScanLine.pCurrent[j] = pP0[j] + m_ScanLine.pSlope[j] * fOfsY;
        }

        // Rasterize
        PixelShader * pPS = m_pRenderer->m_pPixelShader;
        for(Int iY = startY; iY <= endY; ++iY) {
            // Top-left filling-convention, left part
            UInt iX = (fSlopeX < 0.0f) ? MathFn->Ceil(fCurX) : MathFn->Floor(fCurX);
            pPS->ProcessPixel( iX, iY, m_ScanLine.pCurrent );
            fCurX += fSlopeX;
            for(j = 0; j < m_pCFL->GetTotalSize(); ++j)
                m_ScanLine.pCurrent[j] += m_ScanLine.pSlope[j];
        }
    } else {
        if ( fDX < SCALAR_ERROR )
            return; // degenerate segment
        Scalar fInvDX = MathFn->Invert(fDX);

        // Top-left filling-convention, left part
		Int startX = MathFn->Ceil( pPos0->X );
		Int endX = MathFn->Ceil( pPos1->X ) - 1;
        if ( endX < startX )
            return; // degenerate segment
        Scalar fOfsX = ( (Scalar)startX - pPos0->X );

        // Slopes
        Scalar fSlopeY = fDY * fInvDX;
        Scalar fCurY = pPos0->Y + fSlopeY * fOfsX;
        UInt j;
        for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
            m_ScanLine.pSlope[j] = ( pP1[j] - pP0[j] ) * fInvDX;
            m_ScanLine.pCurrent[j] = pP0[j] + m_ScanLine.pSlope[j] * fOfsX;
        }

        // Rasterize
        PixelShader * pPS = m_pRenderer->m_pPixelShader;
        for(Int iX = startX; iX <= endX; ++iX) {
            // Top-left filling-convention, top part
            UInt iY = (fSlopeY < 0.0f) ? MathFn->Ceil(fCurY) : MathFn->Floor(fCurY);
            pPS->ProcessPixel( iX, iY, m_ScanLine.pCurrent );
            fCurY += fSlopeY;
            for(j = 0; j < m_pCFL->GetTotalSize(); ++j)
                m_ScanLine.pCurrent[j] += m_ScanLine.pSlope[j];
        }
    }
}
Void Raster::ScanTriangle( CCFLPtr pPixelA, CCFLPtr pPixelB, CCFLPtr pPixelC )
{
    // IMPORTANT : Fields of pixels A B and C are already scaled by InvW !
    // See _VertexToPixel() in the Renderer class ...

	// Sort pointers by crescent Y
    const Scalar *pP0, *pP1, *pP2;
    const Vertex2 * pSwapPos;
    const Vertex2 * pPos0 = (const Vertex2 *)( m_pCFL->GetField( pPixelA, CFLSEMANTIC_RASTERPOS, 0 ) );
    const Vertex2 * pPos1 = (const Vertex2 *)( m_pCFL->GetField( pPixelB, CFLSEMANTIC_RASTERPOS, 0 ) );
    const Vertex2 * pPos2 = (const Vertex2 *)( m_pCFL->GetField( pPixelC, CFLSEMANTIC_RASTERPOS, 0 ) );
	if (pPos0->Y <= pPos1->Y) {
		if (pPos2->Y <= pPos0->Y) {
            pSwapPos = pPos0;
			pP0 = pPixelC; pPos0 = pPos2;
			pP1 = pPixelA; pPos2 = pPos1;
			pP2 = pPixelB; pPos1 = pSwapPos;
		} else if (pPos1->Y <= pPos2->Y) {
            // no swap
			pP0 = pPixelA;
			pP1 = pPixelB;
			pP2 = pPixelC;
		} else {
            pSwapPos = pPos1;
			pP0 = pPixelA;
			pP1 = pPixelC; pPos1 = pPos2;
			pP2 = pPixelB; pPos2 = pSwapPos;
		}
	} else { // pPos0.Y > pPos1.Y
		if (pPos2->Y <= pPos1->Y) {
            pSwapPos = pPos0;
			pP0 = pPixelC; pPos0 = pPos2;
			pP1 = pPixelB;
			pP2 = pPixelA; pPos2 = pSwapPos;
		} else if (pPos0->Y <= pPos2->Y) {
            pSwapPos = pPos0;
			pP0 = pPixelB; pPos0 = pPos1;
			pP1 = pPixelA; pPos1 = pSwapPos;
			pP2 = pPixelC;
		} else {
            pSwapPos = pPos0;
			pP0 = pPixelB; pPos0 = pPos1;
			pP1 = pPixelC; pPos1 = pPos2;
			pP2 = pPixelA; pPos2 = pSwapPos;
		}
	}

    // Compute inv delta y
	m_ScanConv.fInvDY[0] = (pPos2->Y - pPos0->Y);
	if ( m_ScanConv.fInvDY[0] < SCALAR_ERROR )
		return; // degenerate triangle
	m_ScanConv.fInvDY[0] = MathFn->Invert( m_ScanConv.fInvDY[0] );
	m_ScanConv.fInvDY[1] = (pPos1->Y - pPos0->Y);
	if (m_ScanConv.fInvDY[1] >= SCALAR_ERROR)
		m_ScanConv.fInvDY[1] = MathFn->Invert( m_ScanConv.fInvDY[1] );
	m_ScanConv.fInvDY[2] = (pPos2->Y - pPos1->Y);
	if (m_ScanConv.fInvDY[2] >= SCALAR_ERROR)
		m_ScanConv.fInvDY[2] = MathFn->Invert( m_ScanConv.fInvDY[2] );

    // Compute Mipmap parameters (texcoords' derivates) for all active samplers
    m_pRenderer->m_pPixelShader->ComputeMipmapParameters( (CCFLPtr)pP0,
                                                          (CCFLPtr)pP1,
                                                          (CCFLPtr)pP2 );

	// Find the major edge
    Vector2 vTmp[2];
	vTmp[0] = (*pPos0) - (*pPos2);
	vTmp[1] = (*pPos1) - (*pPos0);
	m_ScanConv.iLeft = (vTmp[0].X * vTmp[1].Y - vTmp[0].Y * vTmp[1].X > 0) ? 0 : 1;

    // Slopes for the major edge
	m_ScanConv.fX[0] = pPos0->X;
	m_ScanConv.fSlopeX[0] = (pPos2->X - pPos0->X) * m_ScanConv.fInvDY[0];
    UInt j;
    for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
        m_ScanConv.pPixels[0][j] = pP0[j];
        m_ScanConv.pSlopePixels[0][j] = (pP2[j] - pP0[j]) * m_ScanConv.fInvDY[0];
    }

    Int iY, startY, endY;
    Scalar fOfsY, fDY;

    // Upper sub-triangle
	if ( m_ScanConv.fInvDY[1] >= SCALAR_ERROR )
	{
        // Slopes for the top edge
        m_ScanConv.fX[1] = pPos0->X;
	    m_ScanConv.fSlopeX[1] = (pPos1->X - pPos0->X) * m_ScanConv.fInvDY[1];
        for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
            m_ScanConv.pPixels[1][j] = pP0[j];
            m_ScanConv.pSlopePixels[1][j] = (pP1[j] - pP0[j]) * m_ScanConv.fInvDY[1];
        }

        // Top-left filling-convention, top part
		startY = MathFn->Ceil( pPos0->Y );
		endY = MathFn->Ceil( pPos1->Y ) - 1;

        // Offset : Y
		fOfsY = ( (Scalar)startY - pPos0->Y );
		m_ScanConv.fX[0] += (m_ScanConv.fSlopeX[0] * fOfsY);
		m_ScanConv.fX[1] += (m_ScanConv.fSlopeX[1] * fOfsY);
        for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
            m_ScanConv.pPixels[0][j] += (m_ScanConv.pSlopePixels[0][j] * fOfsY);
            m_ScanConv.pPixels[1][j] += (m_ScanConv.pSlopePixels[1][j] * fOfsY);
        }

        // Rasterize
		for(iY = startY; iY <= endY; ++iY) {
            m_ScanLine.iY = iY;
			m_ScanLine.fX[m_ScanConv.iLeft] = m_ScanConv.fX[0];
			m_ScanLine.fX[1 - m_ScanConv.iLeft] = m_ScanConv.fX[1];
            for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
                m_ScanLine.pPixels[m_ScanConv.iLeft][j] = m_ScanConv.pPixels[0][j];
                m_ScanLine.pPixels[1 - m_ScanConv.iLeft][j] = m_ScanConv.pPixels[1][j];
            }

            _ScanTriangleLine();

            m_ScanConv.fX[0] += m_ScanConv.fSlopeX[0];
			m_ScanConv.fX[1] += m_ScanConv.fSlopeX[1];
			for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
                m_ScanConv.pPixels[0][j] += m_ScanConv.pSlopePixels[0][j];
                m_ScanConv.pPixels[1][j] += m_ScanConv.pSlopePixels[1][j];
            }
        }
    }

    // Lower sub-triangle
	if ( m_ScanConv.fInvDY[2] >= SCALAR_ERROR )
	{
        // Advance major edge to middle point (if we have done the other edge)
        if ( m_ScanConv.fInvDY[1] >= SCALAR_ERROR ) {
            fDY = (pPos1->Y - pPos0->Y);
			m_ScanConv.fX[0] = pPos0->X + (m_ScanConv.fSlopeX[0] * fDY);
			for(j = 0; j < m_pCFL->GetTotalSize(); ++j)
                m_ScanConv.pPixels[0][j] = pP0[j] + (m_ScanConv.pSlopePixels[0][j] * fDY);
        }

        // Slopes for the bottom edge
        m_ScanConv.fX[1] = pPos1->X;
	    m_ScanConv.fSlopeX[1] = (pPos2->X - pPos1->X) * m_ScanConv.fInvDY[2];
        for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
            m_ScanConv.pPixels[1][j] = pP1[j];
            m_ScanConv.pSlopePixels[1][j] = (pP2[j] - pP1[j]) * m_ScanConv.fInvDY[2];
        }

        // Top-left filling-convention, top part
		startY = MathFn->Ceil( pPos1->Y );
		endY = MathFn->Ceil( pPos2->Y ) - 1;

        // Offset : Y
		fOfsY = ( (Scalar)startY - pPos1->Y );
		m_ScanConv.fX[0] += (m_ScanConv.fSlopeX[0] * fOfsY);
		m_ScanConv.fX[1] += (m_ScanConv.fSlopeX[1] * fOfsY);
        for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
            m_ScanConv.pPixels[0][j] += (m_ScanConv.pSlopePixels[0][j] * fOfsY);
            m_ScanConv.pPixels[1][j] += (m_ScanConv.pSlopePixels[1][j] * fOfsY);
        }

        // Rasterize
		for(iY = startY; iY <= endY; ++iY) {
            m_ScanLine.iY = iY;
			m_ScanLine.fX[m_ScanConv.iLeft] = m_ScanConv.fX[0];
			m_ScanLine.fX[1 - m_ScanConv.iLeft] = m_ScanConv.fX[1];
            for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
                m_ScanLine.pPixels[m_ScanConv.iLeft][j] = m_ScanConv.pPixels[0][j];
                m_ScanLine.pPixels[1 - m_ScanConv.iLeft][j] = m_ScanConv.pPixels[1][j];
            }

            _ScanTriangleLine();

            m_ScanConv.fX[0] += m_ScanConv.fSlopeX[0];
			m_ScanConv.fX[1] += m_ScanConv.fSlopeX[1];
			for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
                m_ScanConv.pPixels[0][j] += m_ScanConv.pSlopePixels[0][j];
                m_ScanConv.pPixels[1][j] += m_ScanConv.pSlopePixels[1][j];
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

Void Raster::_ScanTriangleLine()
{
    // Top-left filling-convention, left part
	Int startX = MathFn->Ceil( m_ScanLine.fX[0] );
	Int endX = MathFn->Ceil( m_ScanLine.fX[1] ) - 1;
	if (endX < startX)
		return;

    // Offset & InvDelta along X
	Scalar fOfsX = ( (Scalar)startX - m_ScanLine.fX[0] );
	Scalar fInvDeltaX = ( m_ScanLine.fX[1] - m_ScanLine.fX[0] );
	if (fInvDeltaX < SCALAR_ERROR)
		fInvDeltaX = 1.0f;
	else
		fInvDeltaX = MathFn->Invert(fInvDeltaX);

    // Slopes & Offsets
    UInt j;
    for(j = 0; j < m_pCFL->GetTotalSize(); ++j) {
        m_ScanLine.pSlope[j] = ( m_ScanLine.pPixels[1][j] - m_ScanLine.pPixels[0][j] ) * fInvDeltaX;
        m_ScanLine.pCurrent[j] = m_ScanLine.pPixels[0][j] + m_ScanLine.pSlope[j] * fOfsX;
    }

	// Rasterize
    PixelShader * pPS = m_pRenderer->m_pPixelShader;
    pPS->ProcessPixel( startX, m_ScanLine.iY, m_ScanLine.pCurrent );
    for(j = 0; j < m_pCFL->GetTotalSize(); ++j)
        m_ScanLine.pCurrent[j] += m_ScanLine.pSlope[j];
    if ( !( m_pRenderer->_GetStateTransform()->UseWireframe() ) ) {
        for(Int iX = startX + 1; iX < endX; ++iX) {
            pPS->ProcessPixel( iX, m_ScanLine.iY, m_ScanLine.pCurrent );
            for(j = 0; j < m_pCFL->GetTotalSize(); ++j)
                m_ScanLine.pCurrent[j] += m_ScanLine.pSlope[j];
        }
    }
    pPS->ProcessPixel( endX, m_ScanLine.iY, m_ScanLine.pCurrent );
}

