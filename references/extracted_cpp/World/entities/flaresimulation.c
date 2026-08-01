// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/flaresimulation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FlareSimulation : Managed
{
    protected Particle          m_ParMainFire;
    protected EffectSound       m_BurningSound;
    protected FlareLight        m_FlareLight;
    const static float          MAX_FARLIGHT_DIST = 40;
    const static float          MIN_FARLIGHT_DIST = 5; 
    
    static ref NoiseParams      m_NoisePar; // Contains the noise data ( template and strength )
    float                       m_LastNoiseTime = -1;
    float                       m_NoiseTimer = 0;
    const float                 NOISE_DELAY = 5; // How much time between two consecutive noise pings
    
    // flare effect rotation
    protected const float       FLARE_SPIN_RATE = 1.15;     // in degrees per simul tick
    protected const float       FLARE_SPIN_RADIUS = 0.18;   // radius of circling motion
    protected Entity            m_Flare;
    protected vector            m_RotationPoint;
    protected vector            m_FlarePosition;
    protected float             m_RotationDegrees;
    
    static protected typename   m_ScriptedLight;
    static protected int        m_ParticleId;
    
    void FlareSimulation()
    {
        m_ScriptedLight  = FlareLight;
        m_ParticleId     = ParticleList.FLAREPROJ_ACTIVATE;
