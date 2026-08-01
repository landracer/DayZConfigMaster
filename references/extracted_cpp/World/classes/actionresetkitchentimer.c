// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionresetkitchentimer.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionResetKitchenTimerClockCB : ActionSingleUseBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CASingleUseTurnOnPlugged;
