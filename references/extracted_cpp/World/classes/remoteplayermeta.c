// Source: F:/Games/Dayz/scripts/4_world/classes/remoteplayerdebug/remoteplayermeta.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RemotePlayerMeta
{
    eRemoteDebugType    m_DebugType;
    PlayerBase          m_Player;
    
    void RemotePlayerMeta(PlayerBase player, eRemoteDebugType type )
    {
        m_Player = player;
        m_DebugType = type;
