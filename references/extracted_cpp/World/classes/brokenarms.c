// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/brokenarms.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BrokenArmsMdfr: ModifierBase
{
    private const   float   HEALTHY_ARM = 80;
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_BROKEN_ARMS;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
