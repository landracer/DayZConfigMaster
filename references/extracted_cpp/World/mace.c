// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/mace.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Mace extends Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionMineRock1H);
