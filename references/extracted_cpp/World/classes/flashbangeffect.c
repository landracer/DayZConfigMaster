// Source: F:/Games/Dayz/scripts/4_world/classes/explosions/flashbangeffect.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FlashbangEffect
{
    protected const float                       ALPHA_MIN = 0.0;
    protected const float                       ALPHA_MAX = 1.0;
    
    protected const float                       SOUND_DEFER_TIME = 0.4;         
 
    protected float                             m_HitDuration;
    protected float                             m_BreakPoint;
    protected float                             m_TimeActive;
    protected float                             m_DayTimeToggle;
    
    protected float                             m_AlphaMaxActual; //actual max alpha of the effect
    protected float                             m_SoundMaxActual; //actual max volume of the sound
    protected float                             m_ProgressMultiplier; 
    
    protected bool                              m_Visual;
    protected bool                              m_Initialized;
    
    protected PlayerBase                        m_Player;
    protected EffectSound                       m_FlashbangEffectSound;
    protected float                             m_SoundStopTime;
    
    protected ref Timer                         m_DeferAttenuation;
    
    protected PPERequester_FlashbangEffects     m_Requester;
    
    void FlashbangEffect(PlayerBase player, bool visual = true)
    {
        m_Player = player;
        m_Visual = visual;
        m_Initialized = false;
 
        m_HitDuration = 8.0;
        m_BreakPoint = 2.5;
        m_AlphaMaxActual = ALPHA_MAX;
        m_SoundMaxActual = 1.0;
        m_ProgressMultiplier = 1.0;
        
        m_FlashbangEffectSound = null;
        
        if (m_Visual)
        {
            Class.CastTo(m_Requester,PPERequesterBank.GetRequester(PPERequester_FlashbangEffects));
            m_Requester.Start();
        }
        
        m_DeferAttenuation = new Timer();
        m_DeferAttenuation.Run(SOUND_DEFER_TIME, this, &quot;PlaySound&quot;, null, false);
        
        m_DayTimeToggle = 5; 
        if ( g_Game.GetDayTime() >= 22.0 || g_Game.GetDayTime() < 7.0 )
        {
            m_DayTimeToggle = 10;
        }
