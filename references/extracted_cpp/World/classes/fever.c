// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/fever.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FeverMdfr: ModifierBase
{
    private float m_Time;
    private float m_NextEvent;
    
    static const float EVENT_INTERVAL_MIN = 12;
    static const float EVENT_INTERVAL_MAX = 18;
    
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_FEVER;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE_SHORT;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_FEVER;
