// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/health.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HealthMdfr: ModifierBase
{
    private float   m_LastHealthLevel;
    private float   m_LastBloodLevel;
    
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_HEALTH;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        DisableDeactivateCheck();
