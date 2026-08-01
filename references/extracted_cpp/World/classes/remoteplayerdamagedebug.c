// Source: F:/Games/Dayz/scripts/4_world/classes/remoteplayerdebug/remoteplayerdamagedebug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RemotePlayerDamageDebug
{
    const int MAX_DAMAGE_RECORDS = 5;
    PlayerBase m_Player;
    bool m_ChangedSinceSerialization;
    
    ref array<ref DamageData> m_DamageList = new array<ref DamageData>;
    
    void RemotePlayerDamageDebug(PlayerBase player)
    {
        m_Player = player;
