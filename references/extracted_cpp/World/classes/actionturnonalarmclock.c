// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionturnonalarmclock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOnAlarmClockCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUseTurnOnPlugged;
