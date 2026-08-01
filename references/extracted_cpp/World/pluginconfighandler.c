// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginconfighandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginConfigHandler extends PluginFileHandler
{   
    ref array<ref CfgParam> m_CfgParams;
    
    void PluginConfigHandler()
    {
        m_CfgParams = new array<ref CfgParam>;
        
        LoadConfigFile();
