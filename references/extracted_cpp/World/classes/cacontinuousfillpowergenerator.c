// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousfillpowergenerator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousFillPowerGenerator : CAContinuousFill
{
    void CAContinuousFillPowerGenerator( float quantity_filled_per_second , int liquid_type )
    {
        m_QuantityFilledPerSecond = quantity_filled_per_second;
        m_liquid_type = liquid_type;
