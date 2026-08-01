// Source: F:/Games/Dayz/scripts/4_world/static/miscgameplayfunctions.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TurnItemIntoItemLambda extends ReplaceItemWithNewLambda
{
    bool m_TransferAgents;
    bool m_TransferVariables;
    bool m_TransferHealth;
    bool m_ExcludeQuantity;
    float m_quantity_override;
    
    void TurnItemIntoItemLambda (EntityAI old_item, string new_item_type, PlayerBase player)
    {
        SetTransferParams();
