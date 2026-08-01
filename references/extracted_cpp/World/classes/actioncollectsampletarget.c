// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actioncollectsampletarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCollectSampleTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.COLLECT_SAMPLE);
