// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/saline.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SalineMdfr: ModifierBase
{
    float m_RegenTime;
    override void Init()
    {
        m_TrackActivatedTime = true;
        m_IsPersistent = true;
        m_ID                    = eModifiers.MDF_SALINE;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        m_RegenTime = CalculateRegenTime();
        DisableActivateCheck();
