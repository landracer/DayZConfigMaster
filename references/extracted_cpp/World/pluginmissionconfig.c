// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginconfighandler/pluginmissionconfig.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginMissionConfig extends PluginConfigHandler
{   
    protected const string FILE_NAME                = &quot;scene_editor.cfg&quot;;
    protected const string SCENE_EDITOR_SCENE       = &quot;scene_load&quot;;
    
    //========================================
    // GetInstance
    //========================================
    static PluginMissionConfig GetInstance()
    {
        return PluginMissionConfig.Cast( GetPlugin(PluginMissionConfig) );
