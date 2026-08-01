// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/pluginuniversaltemperaturesourceserver.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginUniversalTemperatureSourceServer extends PluginBase
{
    const int INTERVAL          = 1;        
    const float LOOKUP_RADIUS   = 20;       
        
    protected float m_AccuTime;
    protected ref set<PlayerBase> m_ClientList;
 
    protected ref array<ref UTemperatureSourceDebug> m_UTemperatureSourceDebugs;
 
    void PluginUniversalTemperatureSourceServer()
    {
        m_ClientList                = new set<PlayerBase>();
        m_UTemperatureSourceDebugs  = new array<ref UTemperatureSourceDebug>();
