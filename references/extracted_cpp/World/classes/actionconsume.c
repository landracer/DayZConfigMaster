// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionconsume.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionConsumeCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DEFAULT, UATimeSpent.DEFAULT);
