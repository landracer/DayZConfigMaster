// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/salmonella.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SalmonellaMdfr: ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 60;
    static const int AGENT_THRESHOLD_DEACTIVATE = 20;
 
    static const int CHANCE_OF_VOMIT = 10;          // base vomit chance
    static const int CHANCE_OF_VOMIT_AGENT = 20;    // adjusted by the agent count
    static const int WATER_DRAIN_FROM_VOMIT = 450;
    static const int ENERGY_DRAIN_FROM_VOMIT = 310;
    static const float STOMACH_MIN_VOLUME = 200;    // min volume of stomach for vomit symptom
    
    static const float EVENT_INTERVAL_MIN = 12;
    static const float EVENT_INTERVAL_MAX = 18;
 
    private float m_Time;
    private float m_NextEvent;
    private float m_ExhaustionTimer;
    
    private bool m_Exhaustion;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_SALMONELLA;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
