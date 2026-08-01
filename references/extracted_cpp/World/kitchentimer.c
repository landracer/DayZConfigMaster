// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/kitchentimer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class KitchenTimer : ClockBase
{
    const string        RINGING_SOUND               = &quot;KitchenTimer_Ring_Loop_SoundSet&quot;;
    const string        DESTROYED_SOUND             = &quot;AlarmClock_Destroyed_SoundSet&quot;;
    const string        HIT_SOUND                   = &quot;AlarmClock_Hit_SoundSet&quot;;
    const string        WORKING_SOUND               = &quot;KitchenTimer_Ticking_Loop_SoundSet&quot;;
    
    EffectSound         m_RingingStopSound;
    static ref NoiseParams  m_NoisePar;
    static NoiseSystem      m_NoiseSystem;
 
    int                 m_AlarmInSecs;
    
    override void Init()
    {
        super.Init();
    
        if (g_Game.IsServer())
        {
            m_NoiseSystem = g_Game.GetNoiseSystem();
            if ( m_NoiseSystem && !m_NoisePar)
            {
                // Create and load noise parameters
                m_NoisePar = new NoiseParams;
                m_NoisePar.LoadFromPath(&quot;cfgVehicles &quot; + GetType() + &quot; NoiseKitchenTimer&quot;);
            }
        }
