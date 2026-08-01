// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/navyuniformjacket.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NavyUniformJacket : Top_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
