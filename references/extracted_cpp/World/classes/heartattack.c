// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/conditions/heartattack.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HeartAttackMdfr: ModifierBase
{
    private const float     HEALTH_DECREMENT_PER_SEC = -0.2;
    private const float     SHOCK_DECREMENT_PER_SEC = -2;
    private const float     SHOCK_LIMIT = 0;
        
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_HEART_ATTACK;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        m_IsPersistent = true;
        DisableDeactivateCheck();
        DisableActivateCheck();
