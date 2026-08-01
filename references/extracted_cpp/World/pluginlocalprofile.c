// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginfilehandler/pluginlocalprofile.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginLocalProfile extends PluginFileHandler
{
    ref map<string, string>                     m_ConfigParams;
    ref map<string, ref TStringArray>               m_ConfigParamsArray;
    ref map<string, ref map<string, string>>        m_ConfigParamsInArray;
    ref map<string, ref array<ref map<string, string>>> m_ConfigParamsArrayInArray;
    
    void PluginLocalProfile()
    {
        m_ConfigParams              = new map<string, string>;
        m_ConfigParamsArray         = new map<string, ref TStringArray>;
        m_ConfigParamsInArray       = new map<string, ref map<string, string>>;
        m_ConfigParamsArrayInArray  = new map<string, ref array<ref map<string, string>>>;
