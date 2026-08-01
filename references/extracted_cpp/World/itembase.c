// Source: F:/Games/Dayz/scripts/4_world/entities/itembase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

typedef ItemBase Inventory_Base;
typedef ItemBaseType Inventory_BaseType;
typedef map<typename,ref ActionOverrideData>    TActionAnimOverrideMap;
 
class DummyItem extends ItemBase
{
    override bool CanPutAsAttachment(EntityAI parent)
    {
        return true;
