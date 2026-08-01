// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/ppertutorial.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_TutorialMenu extends PPERequester_MenuBase
{
    protected float m_Gauss;
    
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        m_Gauss = Param1<float>.Cast(par).param1;
        
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,true,m_Gauss,PPEGaussFilter.L_0_TUTORIALS,PPOperators.SET);
