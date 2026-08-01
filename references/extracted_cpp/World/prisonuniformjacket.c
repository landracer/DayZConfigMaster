// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/prisonuniformjacket.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PrisonUniformJacket extends Top_Base
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
