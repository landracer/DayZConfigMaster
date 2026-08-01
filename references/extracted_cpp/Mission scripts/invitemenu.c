// Source: F:/Games/Dayz/scripts/5_mission/gui/invitemenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class InviteMenu extends UIScriptedMenu
{   
    private TextWidget m_LogoutTimeText;
    private MultilineTextWidget m_DescriptionText;
    private ButtonWidget m_bCancel;
    private ButtonWidget m_bCancelConsole;
    private int m_iTime;
    
    private ref FullTimeData m_FullTime;
 
    void InviteMenu()
    {
        m_iTime = 15;
 
        m_FullTime = new FullTimeData();
 
        if (g_Game.GetMission())
        {
            g_Game.GetMission().AddActiveInputExcludes({&quot;menu&quot;});
 
            g_Game.GetMission().GetHud().ShowHudUI(false);
            g_Game.GetMission().GetHud().ShowQuickbarUI(false);
        }
