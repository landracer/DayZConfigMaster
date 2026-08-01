// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cainteractloop.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAInteractLoop : CABase
{   
    protected float             m_LocalTimeElpased;
    protected float             m_TimeElpased;
    protected float             m_TimeToComplete;
    protected float             m_DefaultTimeToComplete;    
    protected ref Param1<float> m_SpentUnits;
    
    void CAInteractLoop( float time_to_complete_action )
    {
        m_DefaultTimeToComplete = time_to_complete_action;
