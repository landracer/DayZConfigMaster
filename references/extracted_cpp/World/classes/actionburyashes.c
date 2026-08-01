// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionburyashes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBuryAshesCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.BURY_ASHES);
