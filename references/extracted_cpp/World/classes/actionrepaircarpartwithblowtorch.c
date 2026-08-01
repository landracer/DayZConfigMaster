// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepaircarpartwithblowtorch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRepairCarPartWithBlowtorchCB : ActionRepairCarPartCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.BASEBUILDING_REPAIR_MEDIUM);
