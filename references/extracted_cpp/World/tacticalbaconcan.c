// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/tacticalbaconcan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class TacticalBaconCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;TacticalBaconCan_Opened&quot;);
