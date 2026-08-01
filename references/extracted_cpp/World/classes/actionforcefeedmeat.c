// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionforcefeedmeat.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceFeedMeatCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_NORMAL,UATimeSpent.DEFAULT);
