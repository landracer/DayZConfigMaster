// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondestroypart.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDestroyPartCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat( UATimeSpent.DEFAULT_DESTROY/ActionDestroyPart.Cast(m_ActionData.m_Action).CYCLES );
