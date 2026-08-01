// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgesick.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeSick extends DisplayElementBadge 
{
    void BadgeSick(PlayerBase player)
    {
        NUM_OF_BITS = 1;
        m_Key   =   NTFKEY_SICK;
        m_Type  =   eDisplayElements.DELM_BADGE_SICK;
