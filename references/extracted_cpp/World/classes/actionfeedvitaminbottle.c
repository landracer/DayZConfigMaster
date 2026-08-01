// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionfeedvitaminbottle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFeedVitaminBottleCB : ActionContinuousBaseCB
{
    private const float QUANTITY_USED_PER_SEC = 1;
    
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC,UATimeSpent.DEFAULT);
