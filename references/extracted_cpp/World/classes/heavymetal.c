// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/heavymetal.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeavyMetalMdfr : ModifierBase
{   
    static const float WATER_LOSS_HEAVYMETAL_MIN = 0.3;
    static const float WATER_LOSS_HEAVYMETAL_MAX = 0.8; 
    
    static const float HEALTH_LOSS_HEAVYMETAL_MIN = 0.03;
    static const float HEALTH_LOSS_HEAVYMETAL_MAX = 0.05;
    
    static const int SYMPTOMFALLOFF_THRESHOLD1 = 200;
    static const int SYMPTOMFALLOFF_THRESHOLD2 = 300;
    static const int SYMPTOMFALLOFF_THRESHOLD3 = 400;
    
    protected float m_NextEvent;
    protected float m_Time;
    
    protected float m_DeafnessTime;
    protected bool m_IsDeaf;
    
    override void Init()
    {
        m_ID = eModifiers.MDF_HEAVYMETAL1;
 
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
