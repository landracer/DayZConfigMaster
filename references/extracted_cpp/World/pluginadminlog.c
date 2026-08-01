// Source: F:/Games/Dayz/scripts/4_world/plugins/pluginbase/pluginadminlog.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PluginAdminLog extends PluginBase         // Class for admin log messages handled by script
{
    string                          m_PlayerName;
    string                          m_Pid;
    vector                          m_Position; 
    string                          m_PlayerPrefix;
    string                          m_PlayerPrefix2;
    string                          m_Message;
    string                          m_DisplayName;
    string                          m_HitMessage;
    float                           m_Distance;
    PlayerBase                      m_Player;
    PlayerBase                      m_Source;
    string                          m_ItemInHands;
    string                          m_PosArray[3];
    int                             m_DotIndex;
    PlayerStat<float>               m_StatWater;
    PlayerStat<float>               m_StatEnergy;
    BleedingSourcesManagerServer    m_BleedMgr;
    // filters
    protected int                   m_HitFilter;
    protected int                   m_PlacementFilter;
    protected int                   m_ActionsFilter;
    protected int                   m_PlayerListFilter;
    
    ref Timer                       m_Timer;
    autoptr array<Man>              m_PlayerArray;  
    const int                       TIMER_PLAYERLIST = GetPlayerListTimer();
    
    static int GetPlayerListTimer()
    {
        return 300; // seconds
