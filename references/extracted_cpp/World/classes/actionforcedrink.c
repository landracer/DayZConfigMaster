// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionforcedrink.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceDrinkCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.DRINK,UATimeSpent.DEFAULT);
