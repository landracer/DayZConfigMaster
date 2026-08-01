// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondrink.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDrinkCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DRINK,UATimeSpent.DEFAULT);
