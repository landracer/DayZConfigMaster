// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actiondefibrilatetarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDefibrilateTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFIBRILATE);
