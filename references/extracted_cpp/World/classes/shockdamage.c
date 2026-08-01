// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/shockdamage.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShockDamageMdfr: ModifierBase
{
    const int UNCONSIOUSS_COOLDOWN_TIME = 60;//in s
    const int UNCONSIOUSS_COOLDOWN_MIN_TIME = 90000;//in miliseconds
    const int UNCONSIOUSS_COOLDOWN_MAX_TIME = 150000;//in miliseconds
    override void Init()
    {
        m_TrackActivatedTime    = false;
        m_ID                    = eModifiers.MDF_SHOCK_DAMAGE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = 0.5;
