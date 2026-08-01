// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/tendency/tendencythirst.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TendencyThirst extends DisplayElementTendency 
{
    void TendencyThirst(PlayerBase player)
    {
        m_Type  =   eDisplayElements.DELM_TDCY_WATER;
        m_Key = NTFKEY_THIRSTY;
