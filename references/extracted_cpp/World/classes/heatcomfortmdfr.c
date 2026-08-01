// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/heatcomfortmdfr.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeatComfortMdfr: ModifierBase
{
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_ID                    = eModifiers.MDF_TEMPERATURE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        DisableDeactivateCheck();
