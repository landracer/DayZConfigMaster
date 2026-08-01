// Source: F:/Games/Dayz/scripts/5_mission/gui/itemdropwarningmenu.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WarningMenuBase : UIScriptedMenu
{
    protected ButtonWidget m_OkButton;
    protected MultilineTextWidget m_Description;
 
    void WarningMenuBase()
    {
        if (g_Game.GetMission())
        {
            g_Game.GetMission().AddActiveInputExcludes({&quot;menu&quot;});
 
            g_Game.GetMission().GetHud().ShowHudUI(false);
            g_Game.GetMission().GetHud().ShowQuickbarUI(false);
        }
