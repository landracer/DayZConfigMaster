// Source: F:/Games/Dayz/scripts/4_world/entities/building/specific/misc_tirepile_burning.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Misc_TirePile_Burning_DE : BuildingSuper
{
    private const float TIMER_HEATING_UPDATE_INTERVAL       = 2;
    private const float PARAM_HEAT_RADIUS                   = 6;
    private const float PARAM_FULL_HEAT_RADIUS              = 4;
    private const float PARAM_MAX_TRANSFERED_TEMPERATURE    = 25;       
    
    protected TirePileLight     m_Light;
    protected Particle          m_ParticleFire;
    protected EffectSound       m_SoundFireLoop;
    
    // UniversalTemperatureSource
    protected ref UniversalTemperatureSource m_UTSource;
    protected ref UniversalTemperatureSourceSettings m_UTSSettings;
    protected ref UniversalTemperatureSourceLambdaConstant m_UTSLTirePile;
    
    protected ref AreaDamageManager m_AreaDamage;
    
    override void EEInit()
    {
        super.EEInit();
        
        #ifndef SERVER
        // object creation - create effects (light, particles, sound)
        if (!m_Light)
        {
            m_Light = TirePileLight.Cast(ScriptedLightBase.CreateLight(TirePileLight, Vector(0,0,0), 1));
            m_Light.AttachOnMemoryPoint(this, m_Light.m_MemoryPoint);
            m_Light.FadeBrightnessTo(m_Light.m_FireBrightness, 0);
            m_Light.FadeRadiusTo(m_Light.m_FireRadius, 0);
        }
        
        if (!m_ParticleFire)
            m_ParticleFire = ParticleManager.GetInstance().PlayOnObject(ParticleList.TIREPILE_FIRE, this, GetMemoryPointPos(&quot;tirepile_fire&quot;), Vector(0,0,0), true);
        
        if (!m_SoundFireLoop)
            PlaySoundSetLoop(m_SoundFireLoop, &quot;HeavyFire_SoundSet&quot;, 1.0, 2.0);
        #endif
        
        if (g_Game.IsServer() || !g_Game.IsMultiplayer())
        {
            m_UTSSettings                       = new UniversalTemperatureSourceSettings();
            m_UTSSettings.m_UpdateInterval      = TIMER_HEATING_UPDATE_INTERVAL;
            m_UTSSettings.m_Updateable          = true;
            m_UTSSettings.m_TemperatureItemCap  = GameConstants.ITEM_TEMPERATURE_NEUTRAL_ZONE_MIDDLE;
            m_UTSSettings.m_TemperatureCap      = PARAM_MAX_TRANSFERED_TEMPERATURE;
            m_UTSSettings.m_RangeFull           = PARAM_FULL_HEAT_RADIUS;
            m_UTSSettings.m_RangeMax            = PARAM_HEAT_RADIUS;
            
            m_UTSLTirePile                      = new UniversalTemperatureSourceLambdaConstant();
            m_UTSource                          = new UniversalTemperatureSource(this, m_UTSSettings, m_UTSLTirePile);
            m_UTSource.SetActive(true);
        }
