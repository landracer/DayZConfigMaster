// Source: F:/Games/Dayz/scripts/4_world/classes/playermodifiers/modifiers/boneregen.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BoneRegenMdfr: ModifierBase
{
    private const   float   MINIMAL_WATER_TO_REGENRATE = 2500;
    private const   float   MINIMAL_ENERGY_TO_REGENRATE = 4000;
    private const   float   MINIMAL_BLOOD_TO_REGENRATE = 5000;
    private const   float   MINIMAL_BLOOD_REQUIRED_TO_REGENERATE_HEALTH = 5000;
    private const   float   BONE_HEALTH_INCREMENT_PER_SEC = 0.25;
    
    override void Init()
    {
        m_TrackActivatedTime = false;
        m_ID                    = eModifiers.MDF_BONE_REGEN;
        m_TickIntervalInactive  = DEFAULT_TICK_TIME_INACTIVE;
        m_TickIntervalActive    = DEFAULT_TICK_TIME_ACTIVE;
