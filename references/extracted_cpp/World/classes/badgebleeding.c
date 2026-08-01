// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgebleeding.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeBleeding extends DisplayElementBadge 
{
    void BadgeBleeding(PlayerBase player)
    {
        NUM_OF_BITS = 1;
        m_Key   =   NTFKEY_BLEEDISH;
        m_Type  =   eDisplayElements.DELM_BADGE_BLEEDING;
        m_IsClientOnly = true;
