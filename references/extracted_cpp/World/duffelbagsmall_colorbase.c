// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/duffelbagsmall_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class DuffelBagSmall_ColorBase : Backpack_Base 
{
    override void OnDebugSpawn()
    {
        GetInventory().CreateInInventory(&quot;Chemlight_Red&quot;);
        EntityAI item = GetInventory().CreateInInventory(&quot;GPSReceiver&quot;);
        item.GetInventory().CreateInInventory(&quot;Battery9V&quot;);
        item = GetInventory().CreateEntityInCargo(&quot;PersonalRadio&quot;);
        item.GetInventory().CreateInInventory(&quot;Battery9V&quot;);
