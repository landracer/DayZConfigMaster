// Source: F:/Games/Dayz/scripts/4_world/classes/transmissionagents/playeragentpool/playeragentpool.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class PlayerAgentPool
{
    const int STORAGE_VERSION = 137;
 
    int m_AgentMask;    
    float m_LastTicked;
    float m_TotalAgentCount;
    PlayerBase m_Player;
    
    ref map<int,float> m_VirusPool;
    
    protected ref map<int, float> m_AgentTemporaryResistance; 
    
    PluginTransmissionAgents m_PluginTransmissionAgents = PluginTransmissionAgents.Cast(GetPlugin(PluginTransmissionAgents));
    
    void PlayerAgentPool(PlayerBase player)
    {
        m_Player            = player;
        m_LastTicked        = 0;
        m_VirusPool         = new map<int,float>();
        
        m_AgentTemporaryResistance = new map<int,float>();
