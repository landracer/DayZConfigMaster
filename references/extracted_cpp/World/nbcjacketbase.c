// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/nbcjacketbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NBCJacketBase extends Top_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
