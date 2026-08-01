// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/fireplacebase/fireplace.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Fireplace extends FireplaceBase
{
    bool m_ContactEventProcessing = false;
    protected bool m_ContactDeletionProcessing = false;
    
    void Fireplace()
    {
        //Particles - default for FireplaceBase
        PARTICLE_FIRE_START     = ParticleList.CAMP_FIRE_START;
        PARTICLE_SMALL_FIRE     = ParticleList.CAMP_SMALL_FIRE;
        PARTICLE_NORMAL_FIRE    = ParticleList.CAMP_NORMAL_FIRE;
        PARTICLE_SMALL_SMOKE    = ParticleList.CAMP_SMALL_SMOKE;
        PARTICLE_NORMAL_SMOKE   = ParticleList.CAMP_NORMAL_SMOKE;
        PARTICLE_FIRE_END       = ParticleList.CAMP_FIRE_END;
        PARTICLE_STEAM_END      = ParticleList.CAMP_STEAM_2END;
        
        SetEventMask( EntityEvent.CONTACT | EntityEvent.TOUCH );

        m_UTSSettings.m_TemperatureItemCap  = GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_MIDDLE;
        m_UTSSettings.m_TemperatureCap      = 20;
        
        m_ThawnSurfaceUnderSupport = true;
