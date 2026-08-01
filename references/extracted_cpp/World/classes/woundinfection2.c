// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/woundinfection2.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WoundInfectStage2Mdfr: ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 250;
    static const int AGENT_THRESHOLD_DEACTIVATE = 0;
 
    static const int PAIN_EVENT_INTERVAL_MIN = 6;
    static const int PAIN_EVENT_INTERVAL_MAX = 12;
    
    
    static const float DAMAGE_PER_SEC = 0.04;
    
    protected float m_NextEvent;
    protected float m_Time;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_WOUND_INFECTION2;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_WOUND_INFECT_2;
