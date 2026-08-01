// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/edible_base/chelatingtablets.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ChelatingTablets : Edible_Base
{
    //Specify this item can only be combined but not split
    override void InitItemVariables()
    {
        super.InitItemVariables();
 
        can_this_be_combined = true;
