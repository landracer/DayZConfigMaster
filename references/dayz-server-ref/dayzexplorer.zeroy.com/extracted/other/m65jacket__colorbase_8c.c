// ======================================================================
// File: m65jacket__colorbase_8c_source.html
// Category: other
// ======================================================================

class M65Jacket_ColorBase extends Top_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionWringClothes);
}
};
class M65Jacket_Black extends M65Jacket_ColorBase {};
class M65Jacket_Khaki extends M65Jacket_ColorBase {};
class M65Jacket_Tan extends M65Jacket_ColorBase {};
class M65Jacket_Olive extends M65Jacket_ColorBase {};