// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuoustransferquantity.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousTransferQuantity : CAContinuousBase
{
    protected float                 m_TargetUnits;
    protected float                 m_SpentQuantity;
    protected float                 m_ItemQuantity;
    protected float                 m_SourceQuantity;
    protected float                 m_QuantityFilledPerSecond;
    protected float                 m_TimeToComplete;
    protected ref Param1<float>     m_SpentUnits;
    
    void CAContinuousTransferQuantity( float quantity_filled_per_second )
    {
        m_QuantityFilledPerSecond = quantity_filled_per_second;
