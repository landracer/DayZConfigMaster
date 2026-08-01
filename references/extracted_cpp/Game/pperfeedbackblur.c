// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperfeedbackblur.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_FeedbackBlur extends PPERequester_MenuBase
{
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,1.0,PPEGaussFilter.L_0_FEEDBACK,PPOperators.SET);
