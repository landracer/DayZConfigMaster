// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/huntingoptic.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HuntingOptic extends ItemOptics
{
    EntityAI m_Parent;
    
    void HuntingOptic()
    {
        HideSelection(&quot;rings_ris&quot;);
        HideSelection(&quot;rings_ris_pilot&quot;);
