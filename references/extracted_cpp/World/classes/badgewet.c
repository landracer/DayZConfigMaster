// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgewet.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeWet extends DisplayElementBadge 
{
    void BadgeWet(PlayerBase player)
    {
        NUM_OF_BITS = 1;
        m_Key   =   NTFKEY_WETNESS;
        m_Type  =   eDisplayElements.DELM_BADGE_WET;
