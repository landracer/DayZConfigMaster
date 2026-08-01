// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/stuffedstomach.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class StuffedStomachMdfr: ModifierBase
{
    protected const int     STUFFED_TRESHOLD = PlayerConstants.BT_STOMACH_VOLUME_LVL3;
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_STUFFED;
        m_TickIntervalInactive  = 1;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
