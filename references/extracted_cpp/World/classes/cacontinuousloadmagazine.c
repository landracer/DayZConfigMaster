// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousloadmagazine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousLoadMagazine : CAContinuousRepeat
{
    protected float                 m_TargetUnits;
    protected float                 m_SpentQuantity;
    protected float                 m_ItemQuantity;
    protected float                 m_ItemMaxQuantity;
    protected float                 m_SourceQuantity;
    protected float                 m_QuantityFilledPerSecond;
    //protected ref Param1<float>       m_SpentUnits;
    
    void CAContinuousLoadMagazine( float time_to_complete_action )
    {
        m_DefaultTimeToComplete = time_to_complete_action;
