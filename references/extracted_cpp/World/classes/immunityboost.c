// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/immunityboost.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ImmunityBoost: ModifierBase
{
    float           m_LastWaterLevel;
    ref HumanMovementState      m_MovementState = new HumanMovementState();
    float m_RegenTime;
    
    override void Init()
    {
        m_TrackActivatedTime    = true;
        m_AnalyticsStatsEnabled = true;
        m_IsPersistent          = true;
        m_ID                    = eModifiers.MDF_IMMUNITYBOOST;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 1;
        m_RegenTime = PlayerConstants.VITAMINS_LIFETIME_SECS;
