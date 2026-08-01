// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperglasses.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//glasses and helmets
class PPERequester_GenericBlackGlassesBase extends PPERequester_GameplayBase
{
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        
        SetTargetValueFloat(PostProcessEffectType.GodRays,PPEGodRays.PARAM_INTENSITY,true,0,PPEGodRays.L_0_GLASSES,PPOperators.LOWEST);
        SetTargetValueColor(PostProcessEffectType.Glow,PPEGlow.PARAM_COLORIZATIONCOLOR,{0.4,0.4,0.4,0.0},PPEGlow.L_23_GLASSES,PPOperators.SUBSTRACT);
