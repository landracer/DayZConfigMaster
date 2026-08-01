// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgelegs.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeLegs extends DisplayElementBadge 
{
    void BadgeLegs(PlayerBase player)
    {
        NUM_OF_BITS = 2;
        m_Key   =   NTFKEY_LEGS;
        m_Type  =   eDisplayElements.DELM_BADGE_LEGS;
