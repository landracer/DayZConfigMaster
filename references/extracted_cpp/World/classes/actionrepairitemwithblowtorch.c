// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepairitemwithblowtorch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRepairItemWithBlowtorchCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.BASEBUILDING_REPAIR_MEDIUM);
