// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionwashhandsitemcontinuous.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWashHandsItemContinuousCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.WASH_HANDS);
