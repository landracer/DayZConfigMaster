// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/morphinemdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MorphineMdfr: ModifierBase
{
    const int LIFETIME = 60;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_MORPHINE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 1;
 
        DisableActivateCheck();
