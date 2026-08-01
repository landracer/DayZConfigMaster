// ======================================================================
// File: paramedicjacket__colorbase_8c_source.html
// Category: other
// ======================================================================

class ParamedicJacket_ColorBase extends Top_Base
{
override void SetActions()
{
super.SetActions();
AddAction(ActionWringClothes);
}
};
class ParamedicJacket_Blue extends ParamedicJacket_ColorBase {};
class ParamedicJacket_Crimson extends ParamedicJacket_ColorBase {};
class ParamedicJacket_Green extends ParamedicJacket_ColorBase {};