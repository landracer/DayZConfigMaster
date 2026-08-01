// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/contamination.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContaminationStage1Mdfr: ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 1;
    static const int AGENT_THRESHOLD_DEACTIVATE = 100;
    
    static const int EVENT_1_INTERVAL_MIN = 20;
    static const int EVENT_1_INTERVAL_MAX = 40;
    
    protected float m_NextEvent;
    protected float m_Time;
    
    override void Init()
    {
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_CONTAMINATION1;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_CONTAMINATION;
