// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/contamination2.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContaminationStage2Mdfr: ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 100;
    static const int AGENT_THRESHOLD_DEACTIVATE = 400;
    
    static const int EVENT_1_INTERVAL_MIN = 20;
    static const int EVENT_1_INTERVAL_MAX = 40;
    
    protected float m_NextEvent1;
    protected float m_Time1;
    
    
    static const int EVENT_2_INTERVAL_MIN = 200;
    static const int EVENT_2_INTERVAL_MAX = 400;
    
    protected float m_NextEvent2;
    protected float m_Time2;
    
    override void Init()
    {
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_CONTAMINATION2;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_CONTAMINATION2;
