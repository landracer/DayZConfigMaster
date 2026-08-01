// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actioncollectbloodtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCollectBloodTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COLLECT_BLOOD);
