// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/toxicity.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ToxicityMdfr: ModifierBase
{
    private const float     TOXICITY_CLEANUP_PER_SEC = 1;
    private const float     VOMIT_THRESHOLD = 70;
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_TOXICITY;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
        DisableDeactivateCheck();
