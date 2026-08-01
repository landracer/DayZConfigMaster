// ======================================================================
// File: displayelementbadge_8c_source.html
// Category: other
// ======================================================================

class DisplayElementBadge extends DisplayElementBase
{
void SetLevel(int level)
{
SetValue(level);
}
override void UpdateHUD()
{
super.UpdateHUD();
m_ModulePlayerStatus.SetBadge(m_Key, m_Value);
}
}