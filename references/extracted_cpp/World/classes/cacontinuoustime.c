// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuoustime.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousTime : CAContinuousBase
{
    protected float             m_LocalTimeElpased;
    protected float             m_TimeElpased;
    protected float             m_AdjustedTimeToComplete;
    protected float             m_DefaultTimeToComplete;    
    protected ref Param1<float> m_SpentUnits;
    
    void CAContinuousTime( float time_to_complete_action )
    {
        m_DefaultTimeToComplete = time_to_complete_action;
