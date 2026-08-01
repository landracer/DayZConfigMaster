// ======================================================================
// File: elementstance_8c_source.html
// Category: other
// ======================================================================

class ElementStance extends DisplayElementBase
{
void ElementStance(PlayerBase player)
{
NUM_OF_BITS = 2;
m_Type = eDisplayElements.DELM_STANCE;
m_IsClientOnly = true;
}
override void UpdateHUD()
{
super.UpdateHUD();
m_ModulePlayerStatus.SetStance( m_Value );
}
}