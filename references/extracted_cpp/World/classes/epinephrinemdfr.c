// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/epinephrinemdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class EpinephrineMdfr: ModifierBase
{
    const int LIFETIME = 60;
    const float STAMINA_DEPLETION_MULTIPLIER = 0;
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_EPINEPHRINE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 1;
        DisableActivateCheck();
