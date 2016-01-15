/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/WorldOfMonsters/WorldOfMonsters.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : WorldOfMonsters, Entry Point
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
#include "WorldOfMonsters.h"

/////////////////////////////////////////////////////////////////////////////////
// WorldOfMonsters implementation
DefineWindowApplication( WorldOfMonsters );

WorldOfMonsters::WorldOfMonsters():
    WindowApplication( TEXT("World Of Monsters"), PIXEL_FMT_RGBA8, 1024, 768, 0 )
{
    // Input
    m_iActionMap = INVALID_OFFSET;

    // Debug
    m_bWireFrame = false;

    m_strFPSText[0] = NULLBYTE;
    m_strCoordsText[0] = NULLBYTE;
    m_strCullerText[0] = NULLBYTE;

    m_pDebugTextBrush = NULL;
    m_pDebugTextFormat = NULL;

    // State
    m_iState = GAME_STATE_INIT;
}
WorldOfMonsters::~WorldOfMonsters()
{
    // nothing to do
}

Bool WorldOfMonsters::OnInitialize()
{
    Assert( m_iState == GAME_STATE_INIT );

    // Input
    _CreateActions();

    // Managers
    GraphicsManager::Create();
    GraphicsFn->Initialize();
    GameplayManager::Create();
    GameplayFn->Initialize();
    EntityManager::Create();
    EntityFn->Initialize();
    InterfaceManager::Create();
    InterfaceFn->Initialize();

    // Debug
    WorldFn->ToggleDebugDraw( true );

    GPU2DColor hDebugTextColor = Color4::White;
    m_pDebugTextBrush = New GPU2DSolidColorBrush( RenderingFn->GetScreen2DContext() );
    m_pDebugTextBrush->SetColor( &hDebugTextColor );
    m_pDebugTextBrush->Bind();

    m_pDebugTextFormat = New GPU2DTextFormat();
    m_pDebugTextFormat->Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f );
    m_pDebugTextFormat->SetTextAlignment( GPU2DTEXT_ALIGNMENT_LEADING );
    m_pDebugTextFormat->SetParagraphAlignment( GPU2DTEXT_PARAGRAPHALIGNMENT_CENTER );

    // Done
    m_iState = GAME_STATE_LOADING;
    return true;
}
Void WorldOfMonsters::OnCleanup()
{
    Assert( m_iState == GAME_STATE_CLEANUP );

    // Debug
    m_pDebugTextFormat->UnBind();
    Delete( m_pDebugTextFormat );
    m_pDebugTextFormat = NULL;

    m_pDebugTextBrush->UnBind();
    Delete( m_pDebugTextBrush );
    m_pDebugTextBrush = NULL;

    WorldFn->ToggleDebugDraw( false );

    // Managers
    InterfaceFn->Cleanup();
    InterfaceManager::Destroy();
    EntityFn->Cleanup();
    EntityManager::Destroy();
    GameplayFn->Cleanup();
    GameplayManager::Destroy();
    GraphicsFn->Cleanup();
    GraphicsManager::Destroy();

    // Input
    _DestroyActions();

    // Done
    m_iState = GAME_STATE_EXIT;
}

Bool WorldOfMonsters::OnFrameMove()
{
    static TimeMeasure fLastTime = 0.0;

    // Initialization
    if ( fLastTime == 0.0 ) {
        // Timing
        fLastTime = SystemFn->TimeAbsolute( TIMEUNIT_SECONDS );

        ////////////////////////////
        return true;
    }

    // Timing
    TimeMeasure fTime = 0.0;
    Scalar fElapsedTime = (Scalar)( SystemFn->TimeRelative( fLastTime, TIMEUNIT_SECONDS, &fTime ) );

    // Update interface
    InterfaceFn->Update();

    // Update world
    WorldFn->UpdateWorld( fElapsedTime );

    // Timing
    fLastTime = fTime;

    // Done
    return true;
}
Void WorldOfMonsters::OnFrameRender()
{
    // Clear scene
    RenderingFn->ClearRenderTarget( RenderingFn->GetScreenRenderTarget(), (const Float*)Color4::DarkGray );
    RenderingFn->ClearDepthStencil( RenderingFn->GetScreenDepthStencil(), true, true, 1.0f );

    // Render World
    WorldFn->RenderWorld();

    // Render GUI
    InterfaceFn->Render();

    // Render Debug text
    Vector3 vPosition = WorldFn->GetWorldCamera()->GetTarget()->WorldTransform.GetTranslate();

    StringFn->Format( m_strFPSText, TEXT("FPS = %2.5f"), GetAverageFPS() );
    StringFn->Format( m_strCoordsText, TEXT("Coords = (%3f, %3f, %3f)"), vPosition.X, vPosition.Y, vPosition.Z );
    StringFn->Format( m_strCullerText, TEXT("Visible leaves = %d"), WorldFn->GetVisibleGroup().GetCount() );

    GPU2DContext * pContext2D = RenderingFn->GetScreen2DContext();

    GPU2DRectangle hDstRect;
    hDstRect.fLeft = 16.0f;
    hDstRect.fTop = 16.0f;
    hDstRect.fRight = hDstRect.fLeft + 256.0f;
    hDstRect.fBottom = hDstRect.fTop + 16.0f;

    pContext2D->Begin();

    pContext2D->DrawText( m_strFPSText, StringFn->Length(m_strFPSText), m_pDebugTextFormat, &hDstRect, m_pDebugTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
    hDstRect.fTop += 16.0f;
    hDstRect.fBottom += 16.0f;
    pContext2D->DrawText( m_strCoordsText, StringFn->Length(m_strCoordsText), m_pDebugTextFormat, &hDstRect, m_pDebugTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );
    hDstRect.fTop += 16.0f;
    hDstRect.fBottom += 16.0f;
    pContext2D->DrawText( m_strCullerText, StringFn->Length(m_strCullerText), m_pDebugTextFormat, &hDstRect, m_pDebugTextBrush, GPU2DTEXT_DRAWOPTION_CLIP );

    pContext2D->End();

    // Present frame
    RenderingFn->Present();
}

Void WorldOfMonsters::OnExit()
{
    GUIFn->OnExit();
}

Bool WorldOfMonsters::OnKeyPress( KeyCode iKey )
{
    GUIFn->OnKeyPress( iKey );

    return true;
}
Bool WorldOfMonsters::OnKeyRelease( KeyCode iKey )
{
    GUIFn->OnKeyRelease( iKey );

    return true;
}

Void WorldOfMonsters::OnMouseMove( UInt iX, UInt iY )
{
    GUIFn->OnMouseMove( Point2(iX, iY) );
}
Bool WorldOfMonsters::OnMouseWheel( UInt iX, UInt iY, Int iWheelDelta )
{
    GUIFn->OnMouseWheel( Point2(iX, iY), iWheelDelta );

    return true;
}
Bool WorldOfMonsters::OnMousePress( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMousePress( Point2(iX, iY), iButton );

    return true;
}
Bool WorldOfMonsters::OnMouseRelease( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseRelease( Point2(iX, iY), iButton );

    return true;
}
Bool WorldOfMonsters::OnMouseDblClick( UInt iX, UInt iY, KeyCode iButton )
{
    GUIFn->OnMouseDblClick( Point2(iX, iY), iButton );

    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void WorldOfMonsters::_ProcessState_Loading()
{
}
Void WorldOfMonsters::_UpdateGameState()
{
    switch( m_iState ) {
        case GAME_STATE_LOADING:
            _ProcessState_Loading();
            m_iState = GAME_STATE_LOGIN;
            break;
        case GAME_STATE_LOGIN:
            break;
        case GAME_STATE_PLAYER_TOWN:
            break;
        case GAME_STATE_MOVE_BUILDING:
            break;
        case GAME_STATE_TEAM_SELECTION:
            break;
        case GAME_STATE_BATTLE_START:
            break;
        case GAME_STATE_BATTLE_RUN:
            break;
        case GAME_STATE_BATTLE_PLAYER_TURN:
            break;
        case GAME_STATE_BATTLE_VICTORY:
            break;
        case GAME_STATE_BATTLE_DEFEAT:
            break;
        default: Assert( false ); break;
    }
}

