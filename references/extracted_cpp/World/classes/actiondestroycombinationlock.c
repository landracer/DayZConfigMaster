// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondestroycombinationlock.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDestroyCombinationLockCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(6.0);
