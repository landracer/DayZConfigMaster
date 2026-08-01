// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/poisoning.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PoisoningMdfr : ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 1;
    static const int AGENT_THRESHOLD_DEACTIVATE = 40;
    
    static const int VOMIT_OCCURRENCES_MIN = 2;
    static const int VOMIT_OCCURRENCES_MAX = 5;
 
    static const int WATER_DRAIN_FROM_VOMIT = 70;
    static const int ENERGY_DRAIN_FROM_VOMIT = 55;
    
    private float m_ExhaustionTimer;
    private bool m_Exhaustion;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_POISONING;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
