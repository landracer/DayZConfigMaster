// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/plugindeveloper/plugintargettemperature.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class PluginTargetTemperature extends PluginBase
{
    protected PlayerBase m_pPlayer;
    
    void Init(PlayerBase player)
    {
        m_pPlayer = player;
