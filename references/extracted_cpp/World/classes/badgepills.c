// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgepills.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgePills extends DisplayElementBadge 
{
    void BadgePills(PlayerBase player)
    {
        NUM_OF_BITS = 1;
        m_Key   =   NTFKEY_PILLS;
        m_Type  =   eDisplayElements.DELM_BADGE_PILLS;
