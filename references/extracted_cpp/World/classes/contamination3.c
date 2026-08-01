// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/diseases/contamination3.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ContaminationStage3Mdfr: ModifierBase
{
    static const int AGENT_THRESHOLD_ACTIVATE = 400;
    static const int AGENT_THRESHOLD_DEACTIVATE = AGENT_THRESHOLD_ACTIVATE - 1;
    static const int AGENT_THRESHOLD_FEVER = 250;
    static const float DAMAGE_PER_SEC = -3;
 
    override void Init()
    {
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_CONTAMINATION3;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE_SHORT;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_CONTAMINATION2;//using the same sync as stage 2 for now
        m_TrackActivatedTime    = true;
