// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionturnoffalarmclock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOffAlarmClockCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUseTurnOnPlugged;
