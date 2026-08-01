// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondisarmexplosivewithremotedetonatorunpaired.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDisarmExplosiveWithRemoteDetonatorUnpairedCB : ActionDisarmExplosiveCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DISARM_EXPLOSIVE_REMOTE_UNPAIRED);
