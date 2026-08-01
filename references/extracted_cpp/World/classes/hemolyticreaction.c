// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/hemolyticreaction.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HemolyticReactionMdfr: ModifierBase
{
    private float       m_RunningTime;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_HEMOLYTIC_REACTION;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        m_IsPersistent = true;
        m_RunningTime = CalculateRunTime();
        
        DisableActivateCheck();
