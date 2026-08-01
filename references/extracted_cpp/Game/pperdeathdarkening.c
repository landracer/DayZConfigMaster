// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperdeathdarkening.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_DeathDarkening extends PPERequester_GameplayBase
{
    protected float m_Value;
    
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        m_Value = Param1<float>.Cast(par).param1;
        //Print(&quot;PPERequester_DeathDarkening | level: &quot; + m_Value);
        m_Value = Easing.EaseInCubic(m_Value);
        m_Value = Math.Lerp(0,-10,m_Value);
        //Print(&quot;PPERequester_DeathDarkening | value: &quot; + value);
        
        SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,m_Value,PPEExposureNative.L_0_DEATH,PPOperators.SET);
