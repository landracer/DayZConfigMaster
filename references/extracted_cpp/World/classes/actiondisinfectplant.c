// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actiondisinfectplant.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionDisinfectPlantCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousDisinfectPlant(UAQuantityConsumed.GARDEN_DISINFECT_PLANT);
