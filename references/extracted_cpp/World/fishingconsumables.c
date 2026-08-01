// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/consumables/fishingconsumables.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FishingHookBase: ItemBase
{
    override void OnDebugSpawn()
    {
        GetInventory().CreateInInventory(&quot;Worm&quot;);
