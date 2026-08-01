// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/rag.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Rag extends ItemBase
{
    override bool CanSwapEntities(EntityAI otherItem, InventoryLocation otherDestination, InventoryLocation destination)
    {
        if (!super.CanSwapEntities(otherItem, otherDestination, destination))
        {
            return false;
        }
 
        if (Torch.Cast(GetHierarchyParent()) && otherItem.IsInherited(Rag))
        {
            return false;
        }
 
        return true;
