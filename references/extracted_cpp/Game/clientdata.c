// Source: F:/Games/Dayz/scripts/3_game/client/clientdata.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ClientData
{
    static ref ScriptInvoker SyncEvent_OnPlayerListUpdate = new ScriptInvoker();
    static ref ScriptInvoker SyncEvent_OnEntityKilled = new ScriptInvoker();
    static ref ScriptInvoker SyncEvent_OnPlayerIgnitedFireplace = new ScriptInvoker();
    
    static ref array<Man>           m_PlayerBaseList = new array<Man>;
    static ref SyncPlayerList       m_PlayerList;
    static ref SyncPlayerList       m_LastNewPlayers;
    
    static ref OnlineServices       m_OnlineServices;
    
    static void ResetClientData()
    {
        if ( m_PlayerBaseList )
            m_PlayerBaseList.Clear();
        if ( m_PlayerList && m_PlayerList.m_PlayerList )
            m_PlayerList.m_PlayerList.Clear();
        if ( m_LastNewPlayers && m_LastNewPlayers.m_PlayerList )
            m_LastNewPlayers.m_PlayerList.Clear();
