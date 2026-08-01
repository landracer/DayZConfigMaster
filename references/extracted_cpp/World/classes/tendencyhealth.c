// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/tendency/tendencyhealth.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TendencyHealth extends DisplayElementTendency 
{
    void TendencyHealth(PlayerBase player)
    {
        m_Type  =   eDisplayElements.DELM_TDCY_HEALTH;
        m_Key = NTFKEY_HEALTHY;
