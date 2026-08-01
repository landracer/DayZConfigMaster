// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionpickberry.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPickBerryCB : ActionInteractLoopBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAInteractLoop(UATimeSpent.DEFAULT_PICK);
