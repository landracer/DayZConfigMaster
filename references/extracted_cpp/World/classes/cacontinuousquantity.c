// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousquantity.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousQuantity : CAContinuousBase
{
    protected float                 m_ItemQuantity;
    protected float                 m_SpentQuantity;
    protected float                 m_ItemMaxQuantity;
    protected float                 m_QuantityUsedPerSecond;
    protected float                 m_AdjustedQuantityUsedPerSecond;
    protected ref Param1<float>     m_SpentUnits;
    
    void CAContinuousQuantity( float quantity_used_per_second )
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
