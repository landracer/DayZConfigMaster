// Source: F:/Games/Dayz/scripts/5_mission/gui/huddebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! PluginDayzPlayerDebugUI 
// *************************************************************************************
class HudDebugEventHandler extends ScriptedWidgetEventHandler
{
    HudDebug m_HudDebug;
    
    void HudDebugEventHandler( HudDebug hud_debug )
    {
        m_HudDebug = hud_debug;
