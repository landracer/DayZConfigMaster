// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousfertilizegardenslot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousFertilizeGardenSlot : CAContinuousQuantity
{
    protected float m_SlotFertilizerNeed;
    protected float m_AlreadyFilledAmount; // amount of fertilizer present within slot during the setup of action
    protected float m_TimeToComplete;
    protected Slot  m_Slot;
    protected string m_Selection;
 
    void CAContinuousFertilizeGardenSlot(float quantity_used_per_second)
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
        m_TimeToComplete = 0;
