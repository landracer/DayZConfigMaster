// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiondropitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDropItemCB : ActionSingleUseBaseCB
{   
    override void CreateActionComponent()
    {
        super.CreateActionComponent();
        EnableCancelCondition(true);
