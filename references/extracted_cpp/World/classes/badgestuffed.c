// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgestuffed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeStuffed extends DisplayElementBadge 
{
    void BadgeStuffed(PlayerBase player)
    {
        NUM_OF_BITS = 2;
        m_Key   =   NTFKEY_STUFFED;
        m_Type  =   eDisplayElements.DELM_BADGE_STUFFED;
