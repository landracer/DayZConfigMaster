// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperlatencyblur.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_LatencyBlur extends PPERequester_MenuBase
{
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,1.0,PPEGaussFilter.L_0_LATENCY,PPOperators.SET);
