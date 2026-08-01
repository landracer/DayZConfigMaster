// Source: F:/Games/Dayz/scripts/3_game/tools/uiscriptedmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-----------------------------------------------------------------------------
class UIMenuPanel: Managed
{
    proto native UIMenuPanel GetSubMenu();
    proto native UIMenuPanel GetParentMenu();
    proto native UIMenuPanel GetVisibleMenu();
    proto native void SetSubMenu(UIMenuPanel submenu);
    proto native void SetParentMenu(UIMenuPanel parent);
    proto native bool CanClose();
    proto native bool CanCloseOnEscape();
    proto native UIScriptedMenu EnterScriptedMenu(int id);
    
    proto native void DestroySubmenu();
    proto native bool IsAnyMenuVisible();
    proto native bool IsVisible();
    proto native bool IsClosing();
 
#ifdef FEATURE_CURSOR
    proto native bool IsCreatedHidden();
#endif
    
    void OnVisibilityChanged(bool isVisible)
    {
