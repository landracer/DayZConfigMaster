// Source: F:/Games/Dayz/scripts/4_world/static/openitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class OpenItem
{
    static void OpenAndSwitch(ItemBase item_tool, ItemBase item_target, PlayerBase player, float specialty_weight = 0)
    {
        array<int> spill_range = new array<int>;
        
        if( item_tool.ConfigIsExisting(&quot;OpenItemSpillRange&quot;) )
        {
            item_tool.ConfigGetIntArray(&quot;OpenItemSpillRange&quot;, spill_range );
        }
        else
        {
            Debug.LogError(&quot;OpenItemSpillRange config parameter missing, default values used ! &quot;);
            Error(&quot;OpenItemSpillRange config parameter missing, default values used !&quot;);
            spill_range.Insert(0);
            spill_range.Insert(100);
        }
        float spill_modificator = Math.RandomIntInclusive( spill_range.Get(0),spill_range.Get(1) ) / 100;
        
        OpenItem.SwitchItems(item_target, player, spill_modificator, specialty_weight);
