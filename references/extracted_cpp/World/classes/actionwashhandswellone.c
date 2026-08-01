// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionwashhandswellone.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWashHandsWellOneCB : ActionInteractLoopBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAInteractLoop(UATimeSpent.WASH_HANDS);
