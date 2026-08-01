// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgeheartbeat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeHeartbeat extends DisplayElementBadge 
{
    void BadgeHeartbeat(PlayerBase player)
    {
        NUM_OF_BITS = 2;
        m_Key   =   NTFKEY_HEARTBEAT;
        m_Type  =   eDisplayElements.DELM_BADGE_HEARTBEAT;
