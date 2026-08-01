// ======================================================================
// File: longtorch_8c_source.html
// Category: other
// ======================================================================

class LongTorch : Torch
{
void LongTorch()
{
m_DecraftResult = "LongWoodenStick";
m_ParticleLocalPos = Vector(0, 0.83, 0);
}
override void SetActions()
{
super.SetActions();
AddAction(ActionClapBearTrapWithThisItem);
}
};