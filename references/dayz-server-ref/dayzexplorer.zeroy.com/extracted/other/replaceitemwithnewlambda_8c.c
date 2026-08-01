// ======================================================================
// File: replaceitemwithnewlambda_8c_source.html
// Category: other
// ======================================================================

class ReplaceItemWithNewLambda : ReplaceItemWithNewLambdaBase
{
PlayerBase m_Player;
int m_IndexQB;
void ReplaceItemWithNewLambda(EntityAI old_item, string new_item_type, PlayerBase player)
{
m_Player = player;
m_IndexQB = -1;
if (m_Player)
m_IndexQB = m_Player.FindQuickBarEntityIndex(old_item);
}
override void CopyOldPropertiesToNew(notnull EntityAI old_item, EntityAI new_item)
{
super.CopyOldPropertiesToNew(old_item, new_item);
if (m_Player && m_IndexQB != -1)
{
m_Player.SetQuickBarEntityShortcut(new_item, m_IndexQB, true);
}
}
override void OnSuccess(EntityAI new_item)
{
super.OnSuccess(new_item);
if (m_Player && m_IndexQB != -1)
{
m_Player.SetQuickBarEntityShortcut(new_item, m_IndexQB, true);
}
}
};