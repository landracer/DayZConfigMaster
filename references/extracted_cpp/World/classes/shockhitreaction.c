// Source: F:/Games/Dayz/scripts/4_world/classes/shockhitreaction/shockhitreaction.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ShockDealtEffect
{
    const float INTENSITY_COEF_OVERALL = 0.8; //intensity percentage multiplier
    const float INTENSITY_COEF_BLUR = 1;
    const float INTENSITY_COEF_COLOR = 0.2;
    const float INTENSITY_COEF_VIGNETTE = 0.7;
    const float DURATION_MIN = 0.6;
    
    ref Param3<float,float,float> m_EffectParam;
    
    float m_HitDuration;
    float m_BreakPoint;
    float m_TimeActive;
    float m_ShockIntensityMax;
    
    void ShockDealtEffect(float intensity_max)
    {
        float duration_coef = Math.Clamp(intensity_max,DURATION_MIN,1);
        m_HitDuration = 1 * duration_coef;
        m_BreakPoint = 0.2 * duration_coef;
        m_ShockIntensityMax = Math.Clamp(intensity_max,0,1);
        
        m_EffectParam = new Param3<float,float,float>(0,0,0);
        //Print(intensity_max);
        //Print(&quot;HitSyncDebug | ShockDealtEffect: &quot; + g_Game.GetPlayer().GetSimulationTimeStamp());
