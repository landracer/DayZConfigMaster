// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/charcoalmdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CharcoalMdfr: ModifierBase
{
    float m_Killrate;
    const int CHARCOAL_LIFETIME = 300;
    const int CHARCOAL_EFFECT_TIME = 100;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_CHARCOAL;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 3;
        m_Killrate = 2.85;  // # of killed agents per sec
        DisableActivateCheck();
