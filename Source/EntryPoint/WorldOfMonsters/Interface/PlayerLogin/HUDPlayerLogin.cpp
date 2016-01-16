/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/PlayerLogin/HUDPlayerLogin.cpp
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : HUDBackboard
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
#include "HUDPlayerLogin.h"

#include "../InterfaceManager.h"

/////////////////////////////////////////////////////////////////////////////////
// HUDLoginInputModel implementation
HUDLoginInputModel::HUDLoginInputModel():
    GUITextInputModel( TEXT("") )
{
    // nothing to do
}
HUDLoginInputModel::~HUDLoginInputModel()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// HUDPasswordInputModel implementation
HUDPasswordInputModel::HUDPasswordInputModel():
    GUITextInputModel( TEXT("") )
{
    // nothing to do
}
HUDPasswordInputModel::~HUDPasswordInputModel()
{
    // nothing to do
}

/////////////////////////////////////////////////////////////////////////////////
// HUDLoginButtonModel implementation
HUDLoginButtonModel::HUDLoginButtonModel( HUDPlayerLogin * pPlayerLogin ):
    GUIButtonModel( TEXT("Login") )
{
    m_pPlayerLogin = pPlayerLogin;
}
HUDLoginButtonModel::~HUDLoginButtonModel()
{
    // nothing to do
}

Void HUDLoginButtonModel::OnPress()
{
    // Login
    Bool bLoggedIn = m_pPlayerLogin->CheckLogin();
    if ( bLoggedIn ) {
        // Load player's town, switch application state

    }
}

/////////////////////////////////////////////////////////////////////////////////
// HUDPlayerLogin implementation
HUDPlayerLogin::HUDPlayerLogin():
    m_hLoginLabelModel(TEXT("Login :")), m_hLoginInputModel(),
    m_hPasswordLabelModel(TEXT("Password :")), m_hPasswordInputModel(),
    m_hLoginButtonModel( this )
{
    m_pWindow = NULL;

    m_pLoginLabel = NULL;
    m_pLoginInput = NULL;
    m_pPasswordLabel = NULL;
    m_pPasswordInput = NULL;
    m_pLoginButton = NULL;

    _CreateWindow();
}
HUDPlayerLogin::~HUDPlayerLogin()
{
    _DestroyWindow();
}

Bool HUDPlayerLogin::CheckLogin() const
{
    // Build login file path
    const GChar * strLogin = m_hLoginInputModel.GetText();
    const GChar * strPassword = m_hPasswordInputModel.GetText();
    UInt iPasswordLength = m_hPasswordInputModel.GetTextLength();

    GChar strFullPath[PATHNAME_LENGTH + 1];
    StringFn->Format( strFullPath, TEXT("%s/%s.dat"), HUD_PLAYERLOGIN_PATH, strLogin );

    // Load login file
    HFile hFile = SystemFn->OpenFile( strFullPath, FILE_READ );
    if ( !(hFile.IsValid()) )
        return false;

    // Extract reference cryptogram
    HashSHA1 arrRefCryptogram;
    Bool bRead = hFile.ReadBytes( arrRefCryptogram, 20 );
    Assert( bRead );

    // Done with login file
    hFile.Close();

    // Encrypt submitted password
    HashSHA1 arrTestCryptogram;
    SHA1Hasher hSHA1;
    hSHA1.Update( strPassword, iPasswordLength );
    hSHA1.GetHash( arrTestCryptogram );

    // Compare cryptograms
    for ( UInt i = 0; i < 20; ++i ) {
        if ( arrTestCryptogram[i] != arrRefCryptogram[i] )
            return false;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////////

Void HUDPlayerLogin::_CreateWindow()
{
    // Create the window
    Rectangle2 hWindowRect;

    m_pWindow = GUIFn->CreateWindow( TEXT("PlayerLogin"), hWindowRect, GUIWINDOW_STYLE_NAKED );
    GUIWidget * pWindowRoot = m_pWindow->GetClientRoot();

    GUIGridLayout hLayout;
    hLayout.GridRows = 12;
    hLayout.GridColumns = 1;
    hLayout.GridArea.TopLeft.X = 0;
    hLayout.GridArea.DownRight.X = 0;
    hLayout.Anchor = GUILAYOUT_ANCHOR_CENTER;
    hLayout.FillX = GUILAYOUT_FILL_RELATIVE;
    hLayout.FillY = GUILAYOUT_FILL_RELATIVE;
    hLayout.RelativeSize.X = 0.7f;
    hLayout.RelativeSize.Y = 0.9f;

    hLayout.GridArea.TopLeft.Y = 0;
    hLayout.GridArea.DownRight.Y = 0;

    m_pLoginLabel = (GUILabel*)( GUIFn->CreateWidget( GUIWIDGET_LABEL, &m_hLoginLabelModel, pWindowRoot, hLayout, TEXT("LoginLabel") ) );

    hLayout.GridArea.TopLeft.Y = 1;
    hLayout.GridArea.DownRight.Y = 3;

    m_pLoginInput = (GUITextInput*)( GUIFn->CreateWidget( GUIWIDGET_TEXTINPUT, &m_hLoginInputModel, pWindowRoot, hLayout, TEXT("LoginInput") ) );

    hLayout.GridArea.TopLeft.Y = 4;
    hLayout.GridArea.DownRight.Y = 4;

    m_pPasswordLabel = (GUILabel*)( GUIFn->CreateWidget( GUIWIDGET_LABEL, &m_hPasswordLabelModel, pWindowRoot, hLayout, TEXT("PasswordLabel") ) );

    hLayout.GridArea.TopLeft.Y = 5;
    hLayout.GridArea.DownRight.Y = 7;

    m_pPasswordInput = (GUITextInput*)( GUIFn->CreateWidget( GUIWIDGET_TEXTINPUT, &m_hPasswordInputModel, pWindowRoot, hLayout, TEXT("PasswordInput") ) );

    hLayout.GridArea.TopLeft.Y = 8;
    hLayout.GridArea.DownRight.Y = 11;

    m_pLoginButton = (GUIButton*)( GUIFn->CreateWidget( GUIWIDGET_BUTTON, &m_hLoginButtonModel, pWindowRoot, hLayout, TEXT("LoginButton") ) );
}
Void HUDPlayerLogin::_DestroyWindow()
{
    // Destroy the window
    GUIFn->DestroyWindow( m_pWindow );
    m_pWindow = NULL;
}

