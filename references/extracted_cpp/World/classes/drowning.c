// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/drowning.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DrowningMdfr: ModifierBase
{
    static const float STAMINA_RECOVERY_MULTIPLIER = 0;
 
    const int   TICK_FREQUENCY = 1;
    
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_ID                    = eModifiers.MDF_DROWNING;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = TICK_FREQUENCY;
        m_SyncID                = eModifierSyncIDs.MODIFIER_SYNC_DROWNING;
