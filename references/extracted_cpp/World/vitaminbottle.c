// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/vitaminbottle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VitaminBottle : Edible_Base
{
    //Specify this item can only be combined but not split
    override void InitItemVariables()
    {
        super.InitItemVariables();
 
        can_this_be_combined = true;
