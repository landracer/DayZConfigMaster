// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuouswaterplant.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousWaterPlant : CAContinuousQuantity
{
    protected float m_PlantThirstyness;
    protected float m_TimeToComplete;
 
    void CAContinuousWaterPlant( float quantity_used_per_second )
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
