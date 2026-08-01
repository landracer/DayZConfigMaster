// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousrepeat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousRepeat : CAContinuousBase
{
    protected float             m_TimeElpased;
    protected float             m_TotalTimeElpased;
    protected float             m_TimeToComplete;
    protected float             m_DefaultTimeToComplete;
    protected ref Param1<float> m_SpentUnits;
    
    void CAContinuousRepeat( float time_to_complete_action )
    {
        m_DefaultTimeToComplete = time_to_complete_action;
