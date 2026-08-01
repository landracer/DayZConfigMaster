// Source: F:/Games/Dayz/scripts/4_world/classes/virtualhud/elements/tendency/tendencyhunger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TendencyHunger extends DisplayElementTendency 
{
    void TendencyHunger(PlayerBase player)
    {
        m_Type  =   eDisplayElements.DELM_TDCY_ENERGY;
        m_Key = NTFKEY_HUNGRY;
