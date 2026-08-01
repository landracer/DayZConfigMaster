// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondismantlepart.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDismantlePartCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        float time = SetCallbackDuration(m_ActionData.m_MainItem);
        m_ActionData.m_ActionComponent = new CAContinuousTime( time );
