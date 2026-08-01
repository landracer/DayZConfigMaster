// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/fireplacebase/ovenindoor.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OvenIndoor extends FireplaceBase
{
    protected float                 m_SmokePosX;
    protected float                 m_SmokePosY;
    protected float                 m_SmokePosZ;
    protected int                   m_FirePointIndex = 1;   //limited to 1 decimal place (1-9)
    
    static const string OVENPOINT_ACTION_SELECTION  = &quot;oven_action&quot;;
    static const string OVENPOINT_FIRE_POSITION     = &quot;oven_point&quot;;
    static const string OVENPOINT_PLACE_ROT         = &quot;oven_rot&quot;;
    static const string OVENPOINT_SMOKE_POSITION    = &quot;oven_smoke&quot;;
    
    void OvenIndoor()
    {
        //Particles - default for FireplaceBase
        PARTICLE_FIRE_START     = ParticleList.OVEN_FIRE_START;
        PARTICLE_SMALL_FIRE     = ParticleList.OVEN_SMALL_FIRE;
        PARTICLE_NORMAL_FIRE    = ParticleList.OVEN_NORMAL_FIRE;
        PARTICLE_SMALL_SMOKE    = ParticleList.HOUSE_SMALL_SMOKE;
        PARTICLE_NORMAL_SMOKE   = ParticleList.HOUSE_NORMAL_SMOKE;
        PARTICLE_FIRE_END       = ParticleList.OVEN_FIRE_END;
        PARTICLE_STEAM_END      = ParticleList.BARREL_FIRE_STEAM_2END;
        
        //register sync variables
        RegisterNetSyncVariableFloat( &quot;m_SmokePosX&quot;, 0, 0, 2 );
        RegisterNetSyncVariableFloat( &quot;m_SmokePosY&quot;, 0, 0, 2 );
        RegisterNetSyncVariableFloat( &quot;m_SmokePosZ&quot;, 0, 0, 2 );
        RegisterNetSyncVariableInt( &quot;m_FirePointIndex&quot;, 0, 9 );
        
        m_LightDistance = 50;
        SetRoofAbove(true);
        
        m_UTSSettings.m_EnableOnTemperatureControl      = true;
        m_UTSSettings.m_ActiveTemperatureThreshold      = 250.0;
        m_UTSSettings.m_InactiveTemperatureThreshold    = 975.0;
