// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousfillfuel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousFillFuel : CAContinuousBase
{
    protected float                 m_ItemQuantity;
    protected float                 m_SpentQuantity;
    protected float                 m_SpentQuantity_total;
    protected float                 m_EmptySpace; //basically free capacity
    protected float                 m_TimeElpased;
    protected float                 m_QuantityUsedPerSecond;
    protected float                 m_AdjustedQuantityUsedPerSecond;
    protected float                 m_DefaultTimeStep;
    protected ref Param1<float>     m_SpentUnits;
    
    protected PlayerBase            m_Player;
    
    void CAContinuousFillFuel( float quantity_used_per_second, float time_to_progress )
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
        m_DefaultTimeStep = time_to_progress;
