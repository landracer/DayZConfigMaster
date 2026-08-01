// Source: F:/Games/Dayz/scripts/3_game/client/syncplayerlist.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SyncPlayerList
{
    ref array<ref SyncPlayer> m_PlayerList;
 
    void CreatePlayerList()
    {
        if (g_Game.IsServer())
        {
            m_PlayerList = new array<ref SyncPlayer>();
            
            array<PlayerIdentity> identities = new array<PlayerIdentity>();
            g_Game.GetPlayerIndentities(identities);
 
            foreach (auto identity : identities)
            {
                SyncPlayer sync_player = new SyncPlayer;
                sync_player.m_Identity = identity;
                sync_player.m_UID = identity.GetPlainId();
                sync_player.m_PlayerName = identity.GetPlainName();
                m_PlayerList.Insert(sync_player);
            }
        }
