// Source: F:/Games/Dayz/scripts/3_game/tools/uimanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UIManager
{
    proto native UIScriptedMenu EnterScriptedMenu(int id, UIMenuPanel parent);
    proto native UIScriptedMenu CreateScriptedMenu(int id, UIMenuPanel parent);
 
    proto native void EnterServerBrowser(UIMenuPanel parentMenu);
 
    proto native UIScriptedMenu ShowScriptedMenu(UIScriptedMenu menu, UIMenuPanel parent);
    proto native void HideScriptedMenu(UIScriptedMenu menu);
 
    proto native Widget GetWidgetUnderCursor();
    proto native bool IsDialogVisible();
    proto native bool IsDialogHiding();
    proto native bool IsModalVisible();
    proto native void CloseSpecificDialog(int id);
    proto native void CloseDialog();
    proto native void HideDialog();
    
    proto native void ShowDialog(string caption, string text, int id, int butts /*DBT_*/, int def/*DBB_*/, int type /*DMT_*/, UIScriptedMenu handler);
    proto native bool ShowCursor(bool visible);
    proto native bool IsCursorVisible();
    proto native bool IsDialogQueued();
    proto native bool ShowQueuedDialog();
    proto native int  GetLoginQueuePosition();
    proto native bool ScreenFadeVisible();
    proto native void ScreenFadeIn(float duration, string text, int backgroundColor, int textColor);
    proto native void ScreenFadeOut(float duration);
    proto native bool IsScaledMode();
    proto native void SetScaledMode(bool enabled);
    
    proto native UIScriptedMenu GetMenu();
    
    bool Back()
    {
        if (IsDialogVisible() == false)
        {
            UIMenuPanel menu = GetMenu();
            if (menu)
            {
                menu.Close();
                return true;
            }       
        }
 
        return false;
