// Source: F:/Games/Dayz/scripts/4_world/static/paintitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PaintItem
{
    static void Paint(ItemBase item_tool, ItemBase item_target, string base_name, PlayerBase player, float specialty_weight = 0)
    {
        string spray_color = item_tool.ConfigGetString(&quot;color&quot;);
        string item_color = item_target.ConfigGetString(&quot;color&quot;);
 
        string new_class_name = base_name + &quot;_&quot; + spray_color;
        PaintItem.SwitchItems(item_target, new_class_name, player);
