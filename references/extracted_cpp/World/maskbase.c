// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/maskbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class MaskBase extends Mask_Base
{
    float m_LowFilterEventTime;
    
    override bool IsGasMask()
    {
        return true;
