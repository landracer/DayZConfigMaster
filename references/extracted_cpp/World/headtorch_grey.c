// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/headtorch_grey.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Headtorch_Grey : Headtorch_ColorBase 
{
    override void OnDebugSpawn()
    {
        GetInventory().CreateInInventory(&quot;Battery9V&quot;);
