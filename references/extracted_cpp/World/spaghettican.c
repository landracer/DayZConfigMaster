// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/spaghettican.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SpaghettiCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;SpaghettiCan_Opened&quot;);
