// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionburnsewtarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionBurnSewTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.SEW_WOUNDS);
