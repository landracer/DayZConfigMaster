// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/tremor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TremorMdfr: ModifierBase
{
    private const float     TREMOR_DECREMENT_PER_SEC = 0.008;
 
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_TREMOR;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
