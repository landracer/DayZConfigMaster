// Source: F:/Games/Dayz/scripts/5_mission/gui/connectiondialogue.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ConnectionDialogue extends UIScriptedMenu
{   
    protected bool m_DebugMonitorHidden;
 
    protected MultilineTextWidget m_Description;
    protected ButtonWidget m_DisconnectBtn;
    #ifdef PLATFORM_CONSOLE
    protected RichTextWidget m_DisconnectBtnLabel;
    protected Widget m_ConsoleToolbar;
    #endif
    
    void ConnectionDialogue() {}
    
    void ~ConnectionDialogue()
    {
        #ifdef PLATFORM_CONSOLE
        if (g_Game.GetMission())
        {
            g_Game.GetMission().GetOnInputDeviceChanged().Remove(OnInputDeviceChanged);
        }
        #endif
