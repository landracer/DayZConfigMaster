// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/replaceitemwithnewlambdabase.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ReplaceItemWithNewLambdaBase
{
    EntityAI m_OldItem;
    string m_NewItemType;
    protected ref InventoryLocation m_OldLocation;
    protected ref InventoryLocation m_NewLocation;
    protected bool m_RemoveFromLocationPassed = false;
    private bool m_RemoveNetworkObjectInfoPassed = false;
 
    void ReplaceItemWithNewLambdaBase(EntityAI old_item, string new_item_type)
    {
        m_OldItem = old_item;
        m_NewItemType = new_item_type;
