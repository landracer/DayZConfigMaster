// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/basebuildingbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//BASE BUILDING BASE
class BaseBuildingBase extends ItemBase
{
    const string        ANIMATION_DEPLOYED          = &quot;Deployed&quot;;
    
    float               m_ConstructionKitHealth;            //stored health value for used construction kit
 
    ref Construction    m_Construction;
    
    bool                m_HasBase;
    //variables for synchronization of base building parts (2x31 is the current limit)
    int                 m_SyncParts01;                              //synchronization for already built parts (31 parts)
    int                 m_SyncParts02;                              //synchronization for already built parts (+31 parts)
    int                 m_SyncParts03;                              //synchronization for already built parts (+31 parts)
    int                 m_InteractedPartId;                         //construction part id that an action was performed on
    int                 m_PerformedActionId;                        //action id that was performed on a construction part
    
    //Sounds
    //build
    const string SOUND_BUILD_WOOD_LOG           = &quot;putDown_WoodLog_SoundSet&quot;;
    const string SOUND_BUILD_WOOD_PLANK         = &quot;putDown_WoodPlank_SoundSet&quot;;
    const string SOUND_BUILD_WOOD_STAIRS        = &quot;putDown_WoodStairs_SoundSet&quot;;
    const string SOUND_BUILD_METAL              = &quot;putDown_MetalPlank_SoundSet&quot;;
    const string SOUND_BUILD_WIRE               = &quot;putDown_BarbedWire_SoundSet&quot;;
    //dismantle
    const string SOUND_DISMANTLE_WOOD_LOG       = &quot;Crash_WoodPlank_SoundSet&quot;;
    const string SOUND_DISMANTLE_WOOD_PLANK     = &quot;Crash_WoodPlank_SoundSet&quot;;
    const string SOUND_DISMANTLE_WOOD_STAIRS    = &quot;Crash_WoodPlank_SoundSet&quot;;
    const string SOUND_DISMANTLE_METAL          = &quot;Crash_MetalPlank_SoundSet&quot;;
    const string SOUND_DISMANTLE_WIRE           = &quot;putDown_BarbedWire_SoundSet&quot;;
    
    protected EffectSound m_Sound;
    
    ref map<string, ref AreaDamageManager> m_DamageTriggers;
    ref array<string> m_HybridAttachments;
    ref array<string> m_Mountables;
    
    // Constructor
    void BaseBuildingBase() 
    {
        m_DamageTriggers = new map<string, ref AreaDamageManager>;
        
        //synchronized variables
        RegisterNetSyncVariableInt( &quot;m_SyncParts01&quot; );
        RegisterNetSyncVariableInt( &quot;m_SyncParts02&quot; );
        RegisterNetSyncVariableInt( &quot;m_SyncParts03&quot; );
        RegisterNetSyncVariableInt( &quot;m_InteractedPartId&quot; );
        RegisterNetSyncVariableInt( &quot;m_PerformedActionId&quot; );
        RegisterNetSyncVariableBool( &quot;m_HasBase&quot; );
        
        //Construction init
        ConstructionInit();
        
        if (ConfigIsExisting(&quot;hybridAttachments&quot;))
        {
            m_HybridAttachments = new array<string>;
            ConfigGetTextArray(&quot;hybridAttachments&quot;, m_HybridAttachments);
        }
        if (ConfigIsExisting(&quot;mountables&quot;))
        {
            m_Mountables = new array<string>;
            ConfigGetTextArray(&quot;mountables&quot;, m_Mountables);
        }
        
        ProcessInvulnerabilityCheck(GetInvulnerabilityTypeString());
