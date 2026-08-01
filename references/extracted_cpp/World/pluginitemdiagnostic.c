// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginitemdiagnostic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginItemDiagnosticEventHandler extends ScriptedWidgetEventHandler
{
    PluginItemDiagnostic m_Owner;
    override bool OnMouseEnter(Widget w, int x, int y)
    {
        if( ButtonWidget.Cast(w))
        {
            g_Game.GetMission().AddActiveInputExcludes({&quot;menu&quot;});
        }
        return true;
 
