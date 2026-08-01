// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actiondefibrilateself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDefibrilateSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFIBRILATE);
