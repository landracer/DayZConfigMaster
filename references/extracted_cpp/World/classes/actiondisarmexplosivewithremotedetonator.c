// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondisarmexplosivewithremotedetonator.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDisarmExplosiveWithRemoteDetonatorCB : ActionDisarmExplosiveCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DISARM_EXPLOSIVE_REMOTE_PAIRED);
