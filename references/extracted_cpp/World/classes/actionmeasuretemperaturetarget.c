// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/medical/actionmeasuretemperaturetarget.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionMeasureTemperatureTargetCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.MEASURE_TEMP);    
