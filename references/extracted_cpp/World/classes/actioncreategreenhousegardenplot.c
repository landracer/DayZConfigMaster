// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actioncreategreenhousegardenplot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCreateGreenhouseGardenPlotCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DIG_GARDEN);
