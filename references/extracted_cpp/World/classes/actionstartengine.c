// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/vehicles/actionstartengine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionStartCarCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.START_ENGINE);
