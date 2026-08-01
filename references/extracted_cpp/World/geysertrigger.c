// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/geysertrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GeyserTrigger : EffectTrigger
{
    const float     MOUTH_ADJUST_RADIUS         = 0.2; // maxium radius geyser effect can move when using randomized position adjustment
    const string    SOUND_BUBBLING              = &quot;Geyser_bubbling_loop_SoundSet&quot;;
    const string    SOUND_ERUPTION              = &quot;Geyser_eruption_loop_SoundSet&quot;;
    const string    SOUND_ERUPTION_START        = &quot;Geyser_eruption_start_SoundSet&quot;;
    const string    SOUND_ERUPTION_TALL         = &quot;Geyser_eruption_tall_loop_SoundSet&quot;;
    const string    SOUND_ERUPTION_TALL_START   = &quot;Geyser_eruption_tall_start_SoundSet&quot;;
    const string    SOUND_ERUPTION_TALL_END     = &quot;Geyser_eruption_tall_splash_SoundSet&quot;;
        
    protected bool              m_bIsDormant;
    protected bool              m_bIsEruptingSoon;
    protected bool              m_bIsErupting;
    protected bool              m_bIsEruptingTall;
    protected float             m_AdjustedX;                            //deprecated
    protected float             m_AdjustedY;                            //deprecated
    protected vector            m_DefaultPosition;
    protected EGeyserState      m_GeyserState = EGeyserState.DORMANT;   // synchronized state
    
    protected ParticleSource    m_GeyserBubblesParticle;
    protected ParticleSource    m_GeyserParticle;
    protected ParticleSource    m_GeyserTallParticle;
    protected ParticleSource    m_GeyserSplashParticle;
    protected EffectSound       m_SoundBubbling;
    protected EffectSound       m_SoundEruption;
    protected EffectSound       m_SoundEruptionStart;
    protected EffectSound       m_SoundEruptionSecondary;
    protected EffectSound       m_SoundEruptionSecondaryStart;
    protected EffectSound       m_SoundEruptionSecondaryEnd;
 
    void GeyserTrigger()
    {
        RegisterNetSyncVariableInt(&quot;m_GeyserState&quot;, 0, 32);
