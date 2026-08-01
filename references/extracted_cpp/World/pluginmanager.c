// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginmanager.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginManager
{   
    private ref array<typename>                 m_PluginRegister;   // list of modules for creation
    private ref map<typename, ref PluginBase>   m_PluginsPtrs;      // plugin, plugin pointer
    
    //=================================
    // Constructor
    //=================================
    void PluginManager()
    {
        m_PluginRegister    = new array<typename>;
        m_PluginsPtrs       = new map<typename, ref PluginBase>;
