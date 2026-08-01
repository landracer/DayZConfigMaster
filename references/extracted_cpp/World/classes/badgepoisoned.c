// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgepoisoned.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgePoisoned extends DisplayElementBadge 
{
    void BadgePoisoned(PlayerBase player)
    {
        NUM_OF_BITS = 1;
        m_Key   =   NTFKEY_POISONED;
        m_Type  =   eDisplayElements.DELM_BADGE_POISONED;
