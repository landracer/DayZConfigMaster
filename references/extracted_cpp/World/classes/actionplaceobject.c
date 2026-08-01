// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionplaceobject.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPlaceObjectCB : ActiondeployObjectCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_PLACE);
