// Source: F:/Games/Dayz/scripts/4_world/classes/playerstats/playerstatspco.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PCOHandlerStats
{
    ref map<int, ref PlayerStatsPCO_Base> m_PCOs = new map<int, ref PlayerStatsPCO_Base>;
    int m_HighestVersion;;
 
    protected Man m_Player;
    
    void PCOHandlerStats(Man player)
    {
        m_Player = player;
 
        RegisterPCO(new PlayerStatsPCO_v100);
        RegisterPCO(new PlayerStatsPCO_v101);
        RegisterPCO(new PlayerStatsPCO_current);
