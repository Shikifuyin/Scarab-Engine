/////////////////////////////////////////////////////////////////////////////////
// File : EntryPoint/Framework/Interface/PlayerLogin/HUDPlayerLogin.h
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Framework, Interface : HUDPlayerLogin
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None.
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Header prelude
#ifndef SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_PLAYERLOGIN_HUDPLAYERLOGIN_H
#define SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_PLAYERLOGIN_HUDPLAYERLOGIN_H

/////////////////////////////////////////////////////////////////////////////////
// Includes
#include "../../Graphics/GraphicsManager.h"

/////////////////////////////////////////////////////////////////////////////////
// Constants definitions

// Login data files
#define HUD_PLAYERLOGIN_PATH TEXT("LoginData")

// Prototypes
class HUDPlayerLogin;

/////////////////////////////////////////////////////////////////////////////////
// The HUDLoginInputModel class
class HUDLoginInputModel : public GUITextInputModel
{
public:
    HUDLoginInputModel();
    virtual ~HUDLoginInputModel();

    //virtual Void OnTextChange();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The HUDPasswordInputModel class
class HUDPasswordInputModel : public GUITextInputModel
{
public:
    HUDPasswordInputModel();
    virtual ~HUDPasswordInputModel();

    //virtual Void OnTextChange();

protected:

};

/////////////////////////////////////////////////////////////////////////////////
// The HUDLoginButtonModel class
class HUDLoginButtonModel : public GUIButtonModel
{
public:
    HUDLoginButtonModel( HUDPlayerLogin * pPlayerLogin );
    virtual ~HUDLoginButtonModel();

    virtual Void OnPress();

protected:
    HUDPlayerLogin * m_pPlayerLogin;
};

/////////////////////////////////////////////////////////////////////////////////
// The HUDPlayerLogin class
class HUDPlayerLogin
{
public:
    HUDPlayerLogin();
    ~HUDPlayerLogin();

    // Window access
    inline GUIWindow * GetWindow() const;

    // Login
    Bool CheckLogin() const;

private:
    // Helpers
    Void _CreateWindow();
    Void _DestroyWindow();

    // Models
    GUILabelModel m_hLoginLabelModel;
    HUDLoginInputModel m_hLoginInputModel;
    GUILabelModel m_hPasswordLabelModel;
    HUDPasswordInputModel m_hPasswordInputModel;
    HUDLoginButtonModel m_hLoginButtonModel;

    // Window elements
    GUIWindow * m_pWindow;

    GUILabel * m_pLoginLabel;
    GUITextInput * m_pLoginInput;
    GUILabel * m_pPasswordLabel;
    GUITextInput * m_pPasswordInput;
    GUIButton * m_pLoginButton;
};

/////////////////////////////////////////////////////////////////////////////////
// Backward Includes (Inlines & Templates)
#include "HUDPlayerLogin.inl"

/////////////////////////////////////////////////////////////////////////////////
// Header end
#endif // SCARAB_ENTRYPOINT_FRAMEWORK_INTERFACE_PLAYERLOGIN_HUDPLAYERLOGIN_H

