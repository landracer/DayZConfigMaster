// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actioncomponents/cacontinuousrepeatfishing.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CAContinuousRepeatFishing : CAContinuousRepeat
{
    protected const int NO_SIGNAL = -1;
    
    bool    m_SignalTriggered = false;
    float   m_SignalStartTime;
    float   m_SignalEndTime;
    float   m_SignalDuration;
    
    void CAContinuousRepeatFishing( float time_to_complete_action )
    {
        m_DefaultTimeToComplete = time_to_complete_action;
