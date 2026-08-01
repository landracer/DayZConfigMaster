// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/policejacket.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PoliceJacket extends Top_Base 
{
    override void SetActions()
    {
        super.SetActions();
        AddAction(ActionWringClothes);
