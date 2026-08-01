// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/fatigue.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FatigueMdfr: ModifierBase
{   
    private float m_Time;
    private float m_NextEvent;
    
    static const float FATIGUE_EVENT_INTERVAL_MIN   = 5;
    static const float FATIGUE_EVENT_INTERVAL_MAX   = 12;
    static const float STAMINA_RECOVERY_MULTIPLIER  = 0.33;
    static const float STAMINA_DEPLETION_MULTIPLIER = 1.33;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_FATIGUE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE_LONG;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
