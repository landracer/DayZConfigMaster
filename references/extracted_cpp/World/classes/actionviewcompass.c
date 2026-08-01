// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionviewcompass.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRaiseAndViewCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime( -1.0 );
        //EnableStateChangeCallback();
