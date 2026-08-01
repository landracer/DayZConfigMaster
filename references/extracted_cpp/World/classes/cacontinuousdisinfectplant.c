// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousdisinfectplant.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousDisinfectPlant : CAContinuousQuantity
{
    protected float m_PlantNeededSpraying;
    protected float m_TimeToComplete;
    protected float m_SpentQuantityTotal;
    protected float m_StartQuantity;
    protected PlantBase m_Plant;
 
    void CAContinuousDisinfectPlant( float quantity_used_per_second )
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
