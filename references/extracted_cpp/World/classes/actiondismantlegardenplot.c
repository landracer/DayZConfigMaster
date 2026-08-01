// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondismantlegardenplot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDismantleGardenPlotCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime( UATimeSpent.DIG_GARDEN );
