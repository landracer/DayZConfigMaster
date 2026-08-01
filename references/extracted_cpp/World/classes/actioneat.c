// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioneat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionEatBigCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_BIG, UATimeSpent.DEFAULT);
