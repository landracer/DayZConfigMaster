// Source: F:/Games/Dayz/scripts/4_world/systems/inventory/replaceitemwithnewlambda.c Source File
// Extracted from DayZ Code Explorer by Zeroy


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
