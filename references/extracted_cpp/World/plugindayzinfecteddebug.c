// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindayzinfecteddebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// *************************************************************************************
// ! PluginDayZInfectedDebugUIHandler 
// *************************************************************************************
class PluginDayZInfectedDebugUIHandler extends ScriptedWidgetEventHandler
{
    override bool OnClick(Widget w, int x, int y, int button)
    {
        super.OnClick(w, x, y, button);
        return m_pPluginInfectedDebug.OnClick(w, x, y, button);
