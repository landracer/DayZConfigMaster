// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/badge/badgefracture.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BadgeFracture extends DisplayElementBadge 
{
    void BadgeFracture(PlayerBase player)
    {
        NUM_OF_BITS = 1;
        m_Key   =   NTFKEY_FRACTURE;
        m_Type  =   eDisplayElements.DELM_BADGE_FRACTURE;
