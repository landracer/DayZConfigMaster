// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/ppermenueffects.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_MenuEffects extends PPERequester_MenuBase
{
    void SetBlurIntensity(float gauss)
    {
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,gauss,PPEGaussFilter.L_0_MENU,PPOperators.SET);
