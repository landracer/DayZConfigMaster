// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/cholera.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CholeraMdfr: ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 250;
    static const int AGENT_THRESHOLD_DEACTIVATE = 50;
    
    static const int CHANCE_OF_VOMIT = 10;
    static const int CHANCE_OF_VOMIT_AGENT = 30;
    static const int WATER_DRAIN_FROM_VOMIT = 450;
    static const int ENERGY_DRAIN_FROM_VOMIT = 310;
    static const float WATER_LOSS_CHOLERA = 0.5;
    static const float WATER_LOSS_CHOLERA_MIN = 0.1;    // min multiplier of WATER_LOSS_CHOLERA
    static const float STOMACH_MIN_VOLUME = 200;        // for vomit symptom
    
    private float m_ExhaustionTimer;
    private bool m_Exhaustion;
 
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_CHOLERA;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
