// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/barbedwire.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BarbedWire extends ItemBase
{
    // Sounds lists
    const static int        SOUNDS_SPARK_COUNT                          = 4;
    const static int        SOUNDS_CUT_COUNT                            = 3;
    const static int        SOUNDS_COLLISION_COUNT                      = 4;
    const static int        SOUNDS_SHOCK_COUNT                          = 4;
    const static float      RANDOM_SPARK_INTERVAL                       = 5.0; // TO DO! Currently not used.
    
    const static string         m_SoundsSpark[SOUNDS_SPARK_COUNT]           = {&quot;electricFenceSpark1&quot;, &quot;electricFenceSpark2&quot;, &quot;electricFenceSpark3&quot;, &quot;electricFenceSpark4&quot;};
    const static string         m_SoundsCut[SOUNDS_CUT_COUNT]               = {&quot;barbedFenceCut1&quot;, &quot;barbedFenceCut2&quot;, &quot;barbedFenceCut3&quot;};
    const static string         m_SoundsCollision[SOUNDS_COLLISION_COUNT]   = {&quot;barbedFenceCollision1&quot;, &quot;barbedFenceCollision2&quot;, &quot;barbedFenceCollision3&quot;, &quot;barbedFenceCollision4&quot;};
    const static string         m_SoundsShock[SOUNDS_SHOCK_COUNT]           = {&quot;electricFenceShock1&quot;, &quot;electricFenceShock2&quot;, &quot;electricFenceShock3&quot;, &quot;electricFenceShock4&quot;};
    const static string         m_SoundBuzzLoop                             = &quot;electricFenceBuzzLoop1&quot;;
    
    SoundOnVehicle m_BuzzSoundLoop;
    
    ref Timer m_SparkEvent;
    protected ref AreaDamageManager m_AreaDamage;
    
    protected bool m_TriggerActive;
    protected bool m_IsPlaced;
    
    //mounting
    protected bool m_IsMounted;
    protected bool m_LastMountedState;
    const string                SOUND_MOUNT = &quot;putDown_BarbedWire_SoundSet&quot;;
    protected EffectSound       m_MountSound;
    
    
    void BarbedWire()
    {
        m_SparkEvent    = new Timer( CALL_CATEGORY_SYSTEM );
        m_TriggerActive = false;
        m_IsPlaced      = false;
        
        //synchronized variables
        RegisterNetSyncVariableBool( &quot;m_IsMounted&quot; );
