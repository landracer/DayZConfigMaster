// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/pluginuniversaltemperaturesourceclient.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginUniversalTemperatureSourceClient extends PluginBase
{
    const int MAX_SIMULTANEOUS_UTS = 10;
 
    protected float m_UTSAverageTemperature;
 
    protected ref array<ref UTemperatureSourceDebug> m_UTemperatureSourceDebugs;
    
    protected ref Widget m_RootWidget[MAX_SIMULTANEOUS_UTS];
    protected TextListboxWidget m_StatListWidgets[MAX_SIMULTANEOUS_UTS];
    protected TextWidget m_HeaderWidget[MAX_SIMULTANEOUS_UTS];
    
    protected PlayerBase m_Player;
    
    void PluginUniversalTemperatureSourceClient()
    {
        m_UTemperatureSourceDebugs = new array<ref UTemperatureSourceDebug>();
