// Source: F:/Games/Dayz/scripts/5_mission/mission/missionserver.c Source File
// Extracted from DayZ Code Explorer by Zeroy


//: string  uid of the player
typedef Param2<int, string> LogoutInfo; 
 
class MissionServer extends MissionBase
{
    ref array<Man> m_Players;
    ref array<ref CorpseData> m_DeadPlayersArray;
    ref map<PlayerBase, ref LogoutInfo> m_LogoutPlayers;
    ref map<PlayerBase, ref LogoutInfo> m_NewLogoutPlayers;
    ref RainProcurementHandler m_RainProcHandler;
    const int SCHEDULER_PLAYERS_PER_TICK = 5;
    int m_currentPlayer;
    int m_RespawnMode;
    
    // -----------------------
    // ARTILLERY SOUNDS SETUP
    // -----------------------
    private float               m_ArtyBarrageTimer = 0;         // This is not to be edited in Init.c this is just to increment time
    
    // Variables to be modified in Init.c
    protected bool              m_PlayArty = false;             // Toggle if Off map artillery sounds are played
    protected float             m_ArtyDelay = 0;                // Set how much time there is between two barrages (in seconds)
    protected int               m_MinSimultaneousStrikes = 0;   // The MIN of simultaneous shots on the map (Will be clamped between 1 and max shots)
    protected int               m_MaxSimultaneousStrikes = 0;   // The MAX of simultaneous shots on the map (Will be clamped between 1 and max amount of coords)
    protected ref array<vector> m_FiringPos;                    // Where we should fire from. On Init set the relevant data
    
    //All Chernarus firing coordinates 
    protected const ref array<vector> CHERNARUS_STRIKE_POS = 
    {
        &quot;-500.00 165.00 5231.69&quot;,
        &quot;-500.00 300.00 9934.41&quot;,
        &quot;10406.86 192.00 15860.00&quot;,
        &quot;4811.75 370.00 15860.00&quot;,
        &quot;-500.00 453.00 15860.00&quot;
