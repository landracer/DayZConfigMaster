// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/immunesystem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ImmuneSystemMdfr: ModifierBase
{
    bool m_HasDisease;
    bool m_HasHealings;
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_IMMUNE_SYSTEM;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        
        DisableDeactivateCheck();
