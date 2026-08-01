// Source: F:/Games/Dayz/scripts/4_world/classes/shockhandler.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShockHandler
{   
    protected const float                   UPDATE_THRESHOLD = 3; //NOTE : The lower, the more precise but the more synchronization
    const float                             VALUE_CHECK_INTERVAL = 0.95; //in seconds
    private const int                       INTENSITY_FACTOR = 1; //How intense the vignette effect will be, the higher the value, the stronger the effect
    private const int                       VIGNETTE_INTENSITY_MAX = 1; //Max BASE intensity of the vignette effect (w/o. bobbing). 0..2, where 2 is full screen coverage
    private const int                       VIGNETTE_INTENSITY_MAX_TOTAL = 2; //Max TOTAL intensity of the vignette effect (w. bobbing). 0..2, where 2 is full screen coverage
    private const float                     SMOOTHING_MAX_INCR = 0.05; //max smoothing change INCREASE per update
    private const float                     SMOOTHING_MAX_DECR = 0.01; //max smoothing change DECREASE per update
    //bobbing constants
    private const float                     PULSE_PERIOD = 0.5; //The time it takes for pulse to do a full cycle
    private const float                     PULSE_AMPLITUDE = 0.05; //This is a multiplier, keep below 1 or expect the unexpected
    
    protected float                         m_Shock;
    protected float                         m_LastEffectIntensityValue; //for interpolation only
    protected float                         m_ShockValueMax;
    protected float                         m_CumulatedShock; 
    private float                           m_TimeSinceLastTick = VALUE_CHECK_INTERVAL + 1;
    private float                           m_ShockMultiplier = 1;
    private float                           m_PrevVignette; //Previous shock-adjecent value (some normalization required). Client sets it only on regular ShockHandler update!
    
    //bobbing effect
    private float                           m_PulseTimer;
    
    //PPE
    PPERequester_TunnelVisionEffects        m_Requester;
    protected ref Param1<float>             m_Param;
    
    protected PlayerBase                    m_Player;
    
    void ShockHandler(PlayerBase player)
    {
        m_Player = player;
        m_Player.m_CurrentShock = m_Player.GetMaxHealth(&quot;&quot;, &quot;Shock&quot;);
        m_ShockValueMax = m_Player.GetMaxHealth(&quot;&quot;, &quot;Shock&quot;);
        m_PrevVignette = m_Player.m_CurrentShock / m_ShockValueMax; //Equivalent to divided by 100
        m_Requester = PPERequester_TunnelVisionEffects.Cast(PPERequesterBank.GetRequester(PPERequester_TunnelVisionEffects));
        m_Param = new Param1<float>(0);
        
        //loegacy stuff
        m_ShockValueThreshold = m_ShockValueMax * 0.95;
