// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/smershvest.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SmershVest extends Vest_Base
{
    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item,slot_name);
        
        if (SmershBag.Cast(item))
        {
            ShowSelection(&quot;Buttpack&quot;);
        }
