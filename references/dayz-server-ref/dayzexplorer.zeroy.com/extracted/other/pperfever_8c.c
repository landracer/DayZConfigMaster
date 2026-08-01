// ======================================================================
// File: pperfever_8c_source.html
// Category: other
// ======================================================================

class PPERequester_FeverEffects extends PPERequester_GameplayBase
{
void SetFeverIntensity(float intensity)
{
SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,intensity,PPEGaussFilter.L_0_FEVER,PPOperators.ADD_RELATIVE);
}
}