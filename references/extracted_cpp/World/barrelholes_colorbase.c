// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/fireplacebase/barrelholes_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BarrelHoles_ColorBase extends FireplaceBase
{
    //Visual animations
    const string ANIMATION_OPENED           = &quot;LidOff&quot;;
    const string ANIMATION_CLOSED           = &quot;LidOn&quot;;
    
    protected bool m_IsOpenedClient         = false;
    
    protected ref OpenableBehaviour m_Openable;
    
    void BarrelHoles_ColorBase()
    {
        //Particles - default for FireplaceBase
        PARTICLE_FIRE_START     = ParticleList.BARREL_FIRE_START;
        PARTICLE_SMALL_FIRE     = ParticleList.BARREL_SMALL_FIRE;
        PARTICLE_NORMAL_FIRE    = ParticleList.BARREL_NORMAL_FIRE;
        PARTICLE_SMALL_SMOKE    = ParticleList.BARREL_SMALL_SMOKE;
        PARTICLE_NORMAL_SMOKE   = ParticleList.BARREL_NORMAL_SMOKE;
        PARTICLE_FIRE_END       = ParticleList.BARREL_FIRE_END;
        PARTICLE_STEAM_END      = ParticleList.BARREL_FIRE_STEAM_2END;
 
        m_Openable = new OpenableBehaviour(false);
        
        //synchronized variables
        RegisterNetSyncVariableBool(&quot;m_Openable.m_IsOpened&quot;);
        
        ProcessInvulnerabilityCheck(GetInvulnerabilityTypeString());
        
        m_LightDistance = 50;
        
        m_UTSSettings.m_TemperatureItemCap      = GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_MIDDLE;
        m_UTSSettings.m_TemperatureCap          = 20;
        
        m_UnderObjectDecalSpawnSettings.m_ScaleMax = 0.44;
        
        m_ThawnSurfaceUnderSupport = true;
