/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/InterfaceManager.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : InterfaceManager
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
#include "InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// InterfaceManager implementation
InterfaceManager::InterfaceManager():
    Manager<InterfaceManager>(),
    m_hColorBrush( GUIFn->GetScreenContext() ), m_hTextBrush( GUIFn->GetScreenContext() ), m_hTextFormat(), m_hDragNDrop()
{
    // nothing to do
}
InterfaceManager::~InterfaceManager()
{
    // nothing to do
}

Void InterfaceManager::Initialize()
{
    // HUD bitmaps
    //m_pSkinBitmap = GraphicsFn->GetBitmap( TEXT("Bitmap_HUD_Skin") );

    // Initialize HUD locations
    //m_arrHUDLocations[HUD_IMAGE_BACKGROUND_] = HUD_LOCATION_BACKGROUND_;

    // Initialize status effect icons locations

    // Initialize skill icons locations

    // Initialize item icons locations

    // Initialize skin colors
    m_hColorBrush.Bind();
    //m_arrHUDColors[HUD_COLOR_] = Color4::White;
    
    // Initialize skin text formats
    m_hTextBrush.Bind();
    m_hTextFormat.Bind( LOCALENAME_EN_US, GPU2DFONTFAMILY_ARIAL, GPU2DFONT_WEIGHT_NORMAL, GPU2DFONT_STYLE_NORMAL, GPU2DFONT_STRETCH_NORMAL, 10.0f, NULL );
    //m_arrTextFormats[HUD_FONT_] = &m_hTextFormat;

    // Create HUD
    m_pBackboardModel = NULL;

    /////////////////////////////////

    m_pWindow = NULL;

    _CreateHeadUpDisplay();

    // Initialize DragNDrop
    m_hDragNDrop.iType = HUD_DRAGTYPE_UNDEFINED;
    m_hDragNDrop.iIdentifier = 0;
}
Void InterfaceManager::Cleanup()
{
    // Destroy HUD
    _DestroyHeadUpDisplay();

    // Destroy HUD skin
    m_hTextFormat.UnBind();
    m_hTextBrush.UnBind();

    m_hColorBrush.UnBind();
}

Void InterfaceManager::Update()
{
    GUIFn->Update();
}

Void InterfaceManager::Render()
{
    GUIFn->Render();
}

Void InterfaceManager::_CreateHeadUpDisplay()
{
    //PCharacter * pPlayer = (PCharacter*)( GameplayFn->GetCharacter(TEXT("Shiki")) );

    // HUD models
    SelectMemory( TEXT("Scratch") );

    m_pBackboardModel = New HUDBackboardModel();

    ////////////////////////////////////////////////

    UnSelectMemory();

    // HUD window
    UInt iWidth, iHeight;
    GUIFn->GetScreenSize( &iWidth, &iHeight );
    m_pWindow = GUIFn->CreateWindow( TEXT("HUD"), Rectangle2( Point2(4,4), iWidth - 4, iHeight - 4 ), GUIWINDOW_STYLE_NAKED );
    GUIWidget * pHUDRoot = m_pWindow->GetClientRoot();

    // HUD layout
    GUIFn->SelectMemory( TEXT("LayoutData") );

    GUIGridLayout hudLayout;
    hudLayout.FillX = GUILAYOUT_FILL_FIXED;
    hudLayout.FillY = GUILAYOUT_FILL_FIXED;
    hudLayout.Anchor = GUILAYOUT_ANCHOR_FIXED;

    ////////////////////////////////////////////////

    GUIFn->UnSelectMemory();

    // HUD setup
    GUIFn->SetBackboardModel( m_pBackboardModel );

    GUIFn->RegisterWindow( m_pWindow );
    m_pWindow->SetEventBlocking( false );
    m_pWindow->SetVisible( true );
}
Void InterfaceManager::_DestroyHeadUpDisplay()
{
    // HUD setup
    GUIFn->UnregisterWindow( m_pWindow );
   
    GUIFn->SetBackboardModel( NULL );

    // HUD window
    GUIFn->DestroyWindow( m_pWindow );
    m_pWindow = NULL;

    // HUD models
    SelectMemory( TEXT("Scratch") );

    /////////////////////////////////////////////////

    Delete( m_pBackboardModel );
    m_pBackboardModel = NULL;

    UnSelectMemory();
}

