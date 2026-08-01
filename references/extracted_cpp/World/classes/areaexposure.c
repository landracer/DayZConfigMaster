// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/areaexposure.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class AreaExposureMdfr: ModifierBase
{
    const int EVENT_1_INTERVAL_MIN = 3;
    const int EVENT_1_INTERVAL_MAX = 5;
    
    const float AGENTS_PER_SEC = 5;
    protected float m_NextEvent1;
    protected float m_Time1;
    
    const int EVENT_2_INTERVAL_MIN = 13;
    const int EVENT_2_INTERVAL_MAX = 18;
    
    
    const float AGENT_DOSE_PER_BS_SEC = 0.33;//how many agents will be injected in one sec per a single bleeding source
    
    protected float m_NextEvent2;
    protected float m_Time2;
    
    
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_AREAEXPOSURE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE_LONG;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE_SHORT;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_ZONE_EXPOSURE;
        DisableActivateCheck();
        DisableDeactivateCheck();
        
