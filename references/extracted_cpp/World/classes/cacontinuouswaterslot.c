// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuouswaterslot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousWaterSlot : CAContinuousQuantity
{
    protected float m_PlantThirstyness;
    protected float m_TimeToComplete;
    protected float m_StartQuantity;
    protected Slot  m_Slot;
 
    void CAContinuousWaterSlot(float quantity_used_per_second)
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
