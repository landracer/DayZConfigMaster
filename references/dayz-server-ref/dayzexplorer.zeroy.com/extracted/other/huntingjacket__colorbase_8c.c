// ======================================================================
// File: huntingjacket__colorbase_8c_source.html
// Category: other
// ======================================================================

class HuntingJacket_ColorBase extends Top_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionWringClothes);
}
};
class HuntingJacket_Autumn extends HuntingJacket_ColorBase {};
class HuntingJacket_Brown extends HuntingJacket_ColorBase {};
class HuntingJacket_Spring extends HuntingJacket_ColorBase {};
class HuntingJacket_Summer extends HuntingJacket_ColorBase {};
class HuntingJacket_Winter extends HuntingJacket_ColorBase {};