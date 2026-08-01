// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionplaceonground.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPlaceOnGroundCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_PLACE);
