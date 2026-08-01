// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionuncoverheadtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUncoverHeadTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
