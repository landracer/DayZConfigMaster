// Source: F:/Games/Dayz/scripts/3_game/ppemanager/requesters/pperinventoryblur.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PPERequester_InventoryBlur extends PPERequester_GameplayBase
{
    override protected void OnStart(Param par = null)
    {
        super.OnStart();
        SetTargetValueFloat(PostProcessEffectType.GaussFilter,PPEGaussFilter.PARAM_INTENSITY,false,0.5,PPEGaussFilter.L_0_INV,PPOperators.SET); //PPOperators.OVERRIDE?
