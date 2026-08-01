// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionwashhandswater.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWashHandsWaterCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.WASH_HANDS);
