// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousquantityrepeat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousQuantityRepeat : CAContinuousBase
{
    protected float                 m_ItemQuantity;
    protected float                 m_SpentQuantity;
    protected float                 m_ItemMaxQuantity;
    protected float                 m_TimeElpased;
    protected float                 m_QuantityUsedPerSecond;
    protected float                 m_AdjustedQuantityUsedPerSecond;
    protected float                 m_DefaultTimeToRepeat;
    protected ref Param1<float>     m_SpentUnits;
    
    void CAContinuousQuantityRepeat( float quantity_used_per_second, float time_to_repeat )
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
        m_DefaultTimeToRepeat = time_to_repeat;
