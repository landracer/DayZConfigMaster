// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncoverheadtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCoverHeadTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
