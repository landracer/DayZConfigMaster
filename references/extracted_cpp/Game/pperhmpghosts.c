// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperhmpghosts.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_HMPGhosts extends PPERequester_GameplayBase
{
    protected bool m_PulseActive;
    protected float m_PulseProgress;
    
    protected ref array<float> m_ChannelWeights = { 0, 0, 0, 0 };
    protected ref array<float> m_ColorMod = { 1, 1, 1, 1 };
    
    protected bool m_Stopping;
    protected bool m_StopNext;
    protected float m_FadeOutTimeTarget;
    protected float m_FadingTimeElapsed;
    protected float m_FadingProgress;
    protected float m_ElapsedTime;
    
    override protected void OnStart(Param par = null)
    {
        super.OnStart(par);
        
        m_Stopping = false;
        m_StopNext = false;
        m_ElapsedTime = 0.0;
        m_FadingTimeElapsed = 0.0;
        m_FadingProgress = 0.0;
        
        //following changes only performed once, on start
        //noise
        float nScaleX = 0.05; //inverse scaling!
        float nScaleY = 0.05; //inverse scaling!
        //ghosts
        float gOffsetX = 30;
        float gOffsetY = 13;
        float gScaleX = 1;  //inverse scaling!
        float gScaleY = 1;  //inverse scaling!
        //color modulation
        m_ColorMod[0] = 0.8;
        m_ColorMod[1] = 0.8;
        m_ColorMod[2] = 0.8;
        m_ColorMod[3] = 0.85;
        
        SetTargetValueFloat(PostProcessEffectType.Ghost,PPEGhost.PARAM_NOISE_SCALE_X,false, nScaleX,PPEGhost.L_4_HMP,PPOperators.LOWEST);
        SetTargetValueFloat(PostProcessEffectType.Ghost,PPEGhost.PARAM_NOISE_SCALE_Y,false, nScaleY,PPEGhost.L_5_HMP,PPOperators.LOWEST);
        SetTargetValueFloat(PostProcessEffectType.Ghost,PPEGhost.PARAM_GHOST_OFFSET_X,false, gOffsetX,PPEGhost.L_6_HMP,PPOperators.ADD);
        SetTargetValueFloat(PostProcessEffectType.Ghost,PPEGhost.PARAM_GHOST_OFFSET_Y,false, gOffsetY,PPEGhost.L_7_HMP,PPOperators.ADD);
        SetTargetValueFloat(PostProcessEffectType.Ghost,PPEGhost.PARAM_GHOST_SCALE_X,false, gScaleX,PPEGhost.L_8_HMP,PPOperators.LOWEST);
        SetTargetValueFloat(PostProcessEffectType.Ghost,PPEGhost.PARAM_GHOST_SCALE_Y,false, gScaleY,PPEGhost.L_9_HMP,PPOperators.LOWEST);
        SetTargetValueColor(PostProcessEffectType.Ghost,PPEGhost.PARAM_GHOST_COLOR_MOD,m_ColorMod,PPEGhost.L_1_HMP,PPOperators.SET);
