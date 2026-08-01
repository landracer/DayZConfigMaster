// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/peachescan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PeachesCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;PeachesCan_Opened&quot;);
