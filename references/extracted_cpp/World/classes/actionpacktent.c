// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionpacktent.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPackTentCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DEPLOY); //Use same time as Deploy
