// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/actioninteractbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionInteractBaseCB : ActionBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAInteract;
