// Source: F:/Games/Dayz/scripts/4_world/classes/playernotifiers/notifiers/warmthnotfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WarmthNotfr: NotifierBase
{
    private const float DEC_TRESHOLD_LOW    = -0.01;
    private const float DEC_TRESHOLD_MED    = -0.12;
    private const float DEC_TRESHOLD_HIGH   = -0.25;
    private const float INC_TRESHOLD_LOW    = 0.01;
    private const float INC_TRESHOLD_MED    = 0.12;
    private const float INC_TRESHOLD_HIGH   = 0.25;
    
    protected ref SimpleMovingAverage<float> m_AverageHeatComfortDeltaBuffer; 
        
    void WarmthNotfr(NotifiersManager manager)
    {
        m_AverageHeatComfortDeltaBuffer = new SimpleMovingAverage<float>(6, 0.0);
