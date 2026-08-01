// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioneatcan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatCanCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_BIG, UATimeSpent.DEFAULT);
