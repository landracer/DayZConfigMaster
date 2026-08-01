// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/food/cannedfood.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//-------------Animal food-----------------------------------------------
class DogFoodCan : Edible_Base
{
    override void Open()
    {
        ReplaceEdibleWithNew(&quot;DogFoodCan_Opened&quot;);
