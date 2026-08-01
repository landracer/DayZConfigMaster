// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginsounddebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginSoundDebug extends PluginBase
{
    override void OnInit()
    {
        m_TickTimer = new Timer();
        m_TickTimer.Run(0.1, this, &quot;OnGUITimer&quot;, NULL, true);
