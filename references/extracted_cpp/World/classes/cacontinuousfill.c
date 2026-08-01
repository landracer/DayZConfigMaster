// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousfill.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousFill : CAContinuousBase
{
    protected float                 m_TargetUnits;
    protected float                 m_SpentQuantity;
    protected float                 m_SpentQuantity_total;
    protected float                 m_ItemQuantity;
    protected float                 m_AdjustedQuantityFilledPerSecond;
    protected float                 m_QuantityFilledPerSecond;
    protected ref Param1<float>     m_SpentUnits;
    protected float                 m_TimeElpased;
    protected float                 m_DefaultTimeStep = 0.25;
    protected int                   m_liquid_type;
    
    void CAContinuousFill( float quantity_filled_per_second , int liquid_type )
    {
        m_QuantityFilledPerSecond = quantity_filled_per_second;
        m_liquid_type = liquid_type;
