// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionuncoverheadself.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUncoverHeadSelfCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COVER_HEAD);
