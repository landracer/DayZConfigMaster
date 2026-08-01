// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/healthregen.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HealthRegenMdfr: ModifierBase
{
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_HEALTH_REGEN;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        DisableDeactivateCheck();
