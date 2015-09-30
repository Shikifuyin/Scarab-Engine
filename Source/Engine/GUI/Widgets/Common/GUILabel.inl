/////////////////////////////////////////////////////////////////////////////////
// File : Engine/GUI/Widgets/Common/GUILabel.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Widget : Common controls, Text Label
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// GUILabelModel implementation
inline const GChar * GUILabelModel::GetText( UInt * outLength ) const {
    if ( outLength != NULL )
        *outLength = m_iLabelLength;
    return m_strText;
}

/////////////////////////////////////////////////////////////////////////////////
// GUILabel implementation
inline GUIWidgetType GUILabel::GetType() const {
    return GUIWIDGET_LABEL;
}
