// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousempty.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousEmpty : CAContinuousBase
{
    protected float                 m_TargetUnits;
    protected float                 m_SpentQuantity;
    protected float                 m_SpentQuantity_total;
    protected float                 m_AdjustedQuantityEmptiedPerSecond;
    protected float                 m_QuantityEmptiedPerSecond;
    protected bool                  m_WringingClothes;
    protected ref Param1<float>     m_SpentUnits;
    protected float                 m_TimeElpased;
    protected float                 m_DefaultTimeStep = 0.25;
    
    void CAContinuousEmpty( float quantity_emptied_per_second )
    {
        m_QuantityEmptiedPerSecond = quantity_emptied_per_second;
