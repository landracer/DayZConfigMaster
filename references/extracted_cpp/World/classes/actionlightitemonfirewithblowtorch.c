// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionlightitemonfirewithblowtorch.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionLightItemOnFireWithBlowtorchCB : ActionLightItemOnFireCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.FIREPLACE_IGNITE);
