// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/wet.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WetMdfr: ModifierBase
{
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_WETNESS;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
