// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionforcefeed.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionForceFeedCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_NORMAL, UATimeSpent.DEFAULT);
