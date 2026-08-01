// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugincameratools.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginCameraTools extends PluginBase
{
    protected static PluginCameraTools m_Instance;
    protected static bool m_IsOpen;
    
    override void OnInit()
    {
        m_Instance = this;
