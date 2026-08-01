// Source: F:/Games/Dayz/scripts/5_mission/gui/logoutmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class LogoutMenu extends UIScriptedMenu
{   
    private TextWidget m_LogoutTimeText;
    private TextWidget m_DescriptionText;
    private ButtonWidget m_bLogoutNow;
    private ButtonWidget m_bCancel;
    #ifdef PLATFORM_CONSOLE
    private ButtonWidget m_bCancelConsole;
    #endif
    private int m_iTime;
    
    private ref FullTimeData m_FullTime;
 
    void LogoutMenu()
    {
        m_iTime = 0;
        g_Game.SetKeyboardHandle(this);
        
        m_FullTime = new FullTimeData();
