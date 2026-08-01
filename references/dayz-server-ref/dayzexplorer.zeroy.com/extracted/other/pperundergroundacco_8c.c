// ======================================================================
// File: pperundergroundacco_8c_source.html
// Category: other
// ======================================================================

class PPERUndergroundAcco extends PPERequester_GameplayBase
{
void SetEyeAccommodation(float value)
{
SetTargetValueFloat(PPEExceptions.EYEACCOM,PPEEyeAccomodationNative.PARAM_INTENSITY,false,value,PPEEyeAccomodationNative.L_0_UNDERGROUND,PPOperators.SET);
}
}