// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/bakedbeanscan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class BakedBeansCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;BakedBeansCan_Opened&quot;);
