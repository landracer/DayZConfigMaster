// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/tunacan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TunaCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;TunaCan_Opened&quot;);
