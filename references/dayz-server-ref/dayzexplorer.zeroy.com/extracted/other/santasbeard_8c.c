// ======================================================================
// File: santasbeard_8c_source.html
// Category: other
// ======================================================================

class SantasBeard extends Mask_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionWringClothes);
}
override protected void InitGlobalExclusionValues()
{
super.InitGlobalExclusionValues();
AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_0);
}
}