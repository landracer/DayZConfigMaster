// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionwritepaper.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionWritePaperCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        EnableStateChangeCallback();
        m_ActionData.m_ActionComponent = new CAContinuousTime(-1.0);
