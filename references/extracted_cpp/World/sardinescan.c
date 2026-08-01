// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/sardinescan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SardinesCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;SardinesCan_Opened&quot;);
