// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/shock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShockMdfr: ModifierBase
{
    private const float     UNCONSCIOUS_LIMIT = 50;
    private const float     SHOCK_INCREMENT_PER_SEC = 1;
    
    override void Init()
    {
        m_ID                    = eModifiers.MDF_SHOCK;
        m_TickIntervalInactive  = 1;
        m_TickIntervalActive    = 0.35;
        //DisableActivateCheck();
        DisableDeactivateCheck();
