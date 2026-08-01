// ======================================================================
// File: usmcpants__colorbase_8c_source.html
// Category: other
// ======================================================================

class USMCPants_ColorBase extends Pants_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionWringClothes);
}
};
class USMCPants_Desert extends USMCPants_ColorBase {};
class USMCPants_Woodland extends USMCPants_ColorBase {};