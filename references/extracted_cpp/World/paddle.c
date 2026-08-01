// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/gear/tools/paddle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Paddle: Inventory_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionClapBearTrapWithThisItem);
