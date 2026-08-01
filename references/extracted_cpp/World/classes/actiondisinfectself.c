// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/medical/actiondisinfectself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDisinfectSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT);
