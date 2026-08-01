// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousquantityliquidtransfer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousQuantityLiquidTransfer : CAContinuousBase
{
    protected int                   m_TendencyDrain; //1 == drain, 0 == pour, -1 == cancel!
    protected float                 m_ItemQuantity;
    protected float                 m_SpentQuantity;
    protected float                 m_SpentQuantity_total;
    protected float                 m_ItemMaxQuantity; //basically free capacity
    protected float                 m_TimeElpased;
    protected float                 m_QuantityUsedPerSecond;
    protected float                 m_AdjustedQuantityUsedPerSecond;
    protected float                 m_DefaultTimeStep;
    protected ref Param1<float>     m_SpentUnits;
    
    void CAContinuousQuantityLiquidTransfer(float quantity_used_per_second, float time_to_progress, int drain = -1) //-1 used as a safeguard; if no valid value arrives, callback is canceled
    {
        m_QuantityUsedPerSecond = quantity_used_per_second;
        m_DefaultTimeStep = time_to_progress;
        m_TendencyDrain = drain;
