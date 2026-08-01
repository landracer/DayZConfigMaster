// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondisarmmine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDisarmMineCB: ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DISARM_MINE);
