// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionsplinttarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionSplintTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.APPLY_SPLINT);
