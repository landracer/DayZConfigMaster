// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperfever.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_FeverEffects extends PPERequester_GameplayBase
{
    void SetFeverIntensity(float intensity)
    {
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,intensity,PPEGaussFilter.L_0_FEVER,PPOperators.ADD_RELATIVE);
