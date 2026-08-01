// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepaircarenginewithblowtorch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRepairCarEngineWithBlowtorchCB : ActionRepairCarEngineCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.BASEBUILDING_REPAIR_MEDIUM);
