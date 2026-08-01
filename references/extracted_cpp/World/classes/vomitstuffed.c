// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/vomitstuffed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VomitStuffedMdfr : ModifierBase
{
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_ID                    = eModifiers.MDF_VOMITSTUFFED;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
