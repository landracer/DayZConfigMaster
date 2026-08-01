// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/burning.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BurningMdfr: ModifierBase
{
    private const float     BURNING_TRESHOLD = 199;
    private const float     HEALTH_DECREMENT_PER_SEC = -5;
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_BURNING;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
