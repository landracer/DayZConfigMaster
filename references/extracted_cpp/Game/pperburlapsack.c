// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperburlapsack.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_BurlapSackEffects extends PPERequester_GameplayBase
{
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        SetTargetValueFloat(PPEExceptions.EXPOSURE,PPEExposureNative.PARAM_INTENSITY,false,-100.0,PPEExposureNative.L_0_BURLAP,PPOperators.SET);
        SetTargetValueFloat(PPEExceptions.EYEACCOM,PPEEyeAccomodationNative.PARAM_INTENSITY,false,0.0,PPEEyeAccomodationNative.L_0_BURLAP,PPOperators.SET);
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_VIGNETTE,false,2.0,PPEGlow.L_25_BURLAP,PPOperators.SET);
        SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_VIGNETTECOLOR,{0.0,0.0,0.0,0.0},PPEGlow.L_26_BURLAP,PPOperators.LOWEST);
        
        if (g_Game && g_Game.GetMission() && g_Game.GetMission().GetEffectWidgets())
        {
            g_Game.GetMission().GetEffectWidgets().AddSuspendRequest(EffectWidgetSuspends.BURLAPSACK);//these should probably be elsewhere, PPEs are mission independent
        }
