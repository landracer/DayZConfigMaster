// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/blinded.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BlindedMdfr: ModifierBase
{
    private const float     BLIND_TIME = 7;
    private float           m_Timer;
 
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_BLINDED;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        DisableActivateCheck();
