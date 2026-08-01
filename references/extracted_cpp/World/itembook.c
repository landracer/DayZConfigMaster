// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/itembook.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ItemBook extends InventoryItemSuper
{
    override event bool OnUseFromInventory(Man owner)
    {
        return false;
