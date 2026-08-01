// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/ppercameraads_opt.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_CameraADS extends PPERequester_GameplayBase
{
    void SetValuesOptics(out array<float> mask_array, out array<float> lens_array, float gauss = 0.0)
    {
        //mask
        g_Game.ResetPPMask();
        if (mask_array.Count() != 4)
        {
            mask_array = {0.0,0.0,0.0,0.0};
        }
        g_Game.AddPPMask(mask_array[0], mask_array[1], mask_array[2], mask_array[3]);
        
        //lens
        if (lens_array.Count() != 4)
        {
            lens_array = {0.0,0.0,0.0,0.0};
        }
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_LENSDISTORT,false,lens_array[0],PPEGlow.L_27_ADS,PPOperators.SET);
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_MAXCHROMABBERATION,false,lens_array[1],PPEGlow.L_28_ADS,PPOperators.SET);
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_LENSCENTERX,false,lens_array[2],PPEGlow.L_29_ADS,PPOperators.SET);
        SetTargetValueFloat(PostProcessEffectType.Glow,PPEGlow.PARAM_LENSCENTERY,false,lens_array[3],PPEGlow.L_30_ADS,PPOperators.SET);
        
        //DOF - no DOF in optics
        SetTargetValueBoolDefault(PPEExceptions.DOF,PPEDOF.PARAM_ENABLE);
        
        //blur
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,false,gauss,PPEGaussFilter.L_0_ADS,PPOperators.ADD_RELATIVE);
