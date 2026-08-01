// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousquantityedible.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousQuantityEdible : CAContinuousQuantityRepeat
{
    protected float     m_InitItemQuantity;
    protected float     m_SpentQuantityTotal;
    protected float     m_QuantityUsedPerSecondStart;
    
    void CAContinuousQuantityEdible(float quantity_used_per_second, float time_to_repeat)
    {
        m_QuantityUsedPerSecondStart = quantity_used_per_second;
 
        m_DefaultTimeToRepeat   = time_to_repeat;
        m_InitItemQuantity      = -1;
        m_SpentQuantityTotal    = 0;
