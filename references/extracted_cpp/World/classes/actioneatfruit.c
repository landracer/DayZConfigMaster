// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioneatfruit.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatFruitCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_NORMAL,UATimeSpent.DEFAULT);
