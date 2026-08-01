// ======================================================================
// File: paintitem_8c_source.html
// Category: other
// ======================================================================

class PaintItem
{
static void Paint(ItemBase item_tool, ItemBase item_target, string base_name, PlayerBase player, float specialty_weight = 0)
{
string spray_color = item_tool.ConfigGetString("color");
string item_color = item_target.ConfigGetString("color");
string new_class_name = base_name + "_" + spray_color;
PaintItem.SwitchItems(item_target, new_class_name, player);
}
static bool CanPaintItem(ItemBase item_tool, ItemBase item_target)
{
string spray_color = item_tool.ConfigGetString("color");
string item_color = item_target.ConfigGetString("color");
if( spray_color != item_color )
{
return true;
}
else
{
return false;
}
}
static void SwitchItems (EntityAI old_item, string new_item, PlayerBase player)
{
MiscGameplayFunctions.TurnItemIntoItemEx(player, new PaintItemLambda(old_item, new_item, player));
}
};
class PaintItemLambda : TurnItemIntoItemLambda
{
void PaintItemLambda (EntityAI old_item, string new_item_type, PlayerBase player) { }
};