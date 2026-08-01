// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperspooky.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// PPE when player is in spooky area trigger
class PPERequester_SpookyAreaTint extends PPERequester_GameplayBase
{
    protected vector m_StartRGB = vector.Zero;
    protected float m_AccumulatedTime = 0;
    protected bool m_FadeIn = false;
    protected bool m_FadeOut = false;
    
    const float FADE_TIME = 3;
    // the end result is 1 - the value set here
    const float R_TARGET = 0.60; // 0.31  79   129 0.50
    const float G_TARGET = 0.28; // 0.80 204   184 0.72
    const float B_TARGET = 0.07; // 0.95 242   239 0.93
    
    override protected void OnStart( Param par = null )
    {
        super.OnStart( par );
        
        m_AccumulatedTime = 0;
        
        m_FadeIn = true;
        m_FadeOut = false;
        
        SetTargetValueFloat(PostProcessEffectType.FilmGrain,PPEFilmGrain.PARAM_SHARPNESS,false,10.0,PPEFilmGrain.L_1_TOXIC_TINT,PPOperators.HIGHEST);
        SetTargetValueFloat(PostProcessEffectType.FilmGrain,PPEFilmGrain.PARAM_GRAINSIZE,false,1.0,PPEFilmGrain.L_2_TOXIC_TINT,PPOperators.LOWEST);
