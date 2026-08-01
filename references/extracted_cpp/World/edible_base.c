// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Edible_Base : ItemBase
{
    const string DIRECT_COOKING_SLOT_NAME   = &quot;DirectCooking&quot;;
 
    const string SOUND_BAKING_START         = &quot;Baking_SoundSet&quot;;
    const string SOUND_BAKING_DONE          = &quot;Baking_Done_SoundSet&quot;;
    const string SOUND_BOILING_START        = &quot;Boiling_SoundSet&quot;;
    const string SOUND_BOILING_DONE         = &quot;Boiling_Done_SoundSet&quot;;
    const string SOUND_DRYING_START         = &quot;Drying_SoundSet&quot;;
    const string SOUND_DRYING_DONE          = &quot;Drying_Done_SoundSet&quot;;
    const string SOUND_BURNING_DONE         = &quot;Food_Burning_SoundSet&quot;;
 
    protected bool m_MakeCookingSounds;
    protected SoundOnVehicle m_SoundCooking; 
    protected EffectSound m_SoundEffectCooking;
    protected string m_SoundPlaying;
    ref FoodStage m_FoodStage;
    protected float m_DecayTimer;
    protected float m_DecayDelta = 0.0;
    protected FoodStageType m_LastDecayStage = FoodStageType.NONE;
    protected ParticleSource    m_HotVaporParticle;
    
    private CookingMethodType m_CookedByMethod;
        
    void Edible_Base()
    {
        if (HasFoodStage())
        {
            m_FoodStage = new FoodStage(this);
            
            RegisterNetSyncVariableInt(&quot;m_FoodStage.m_FoodStageType&quot;,  FoodStageType.NONE, FoodStageType.COUNT);
            RegisterNetSyncVariableFloat(&quot;m_FoodStage.m_CookingTime&quot;,  0, 600, 0);
 
            m_SoundPlaying = &quot;&quot;;
            m_CookedByMethod = CookingMethodType.NONE;
            RegisterNetSyncVariableInt(&quot;m_CookedByMethod&quot;, CookingMethodType.NONE, CookingMethodType.COUNT);
            RegisterNetSyncVariableBool(&quot;m_MakeCookingSounds&quot;);
        }
