// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/unconsciousness.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class UnconsciousnessMdfr: ModifierBase
{
    const int UNCONSIOUSS_COOLDOWN_TIME = 60;//in s
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_AnalyticsStatsEnabled = true;
        m_ID                    = eModifiers.MDF_UNCONSCIOUSNESS;
        m_TickIntervalInactive  = 0.5;
        m_TickIntervalActive    = 0.5;
