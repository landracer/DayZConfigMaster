// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperunconeffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_UnconEffects extends PPERequester_GameplayBase
{
    protected const float FADING_TIME_DEFAULT = 3.0;
    
    protected float m_Intensity;
    protected float m_FadeoutStartIntensity;
    protected bool m_Stopping;
    protected bool m_StopNext;
    protected float m_FadeOutTimeTarget;
    protected float m_FadingTimeElapsed;
    protected float m_FadingProgress;
    
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        m_Intensity = Param1<float>.Cast(par).param1;
        m_Stopping = false;
        m_StopNext = false;
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_VIGNETTE,false,m_Intensity,PPEGlow.L_25_UNCON,PPOperators.HIGHEST);
        SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_VIGNETTECOLOR,{0.0,0.0,0.0,0.0},PPEGlow.L_26_UNCON,PPOperators.LOWEST);
