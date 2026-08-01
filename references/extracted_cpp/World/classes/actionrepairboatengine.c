// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepairboatengine.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionRepairBoatEngineCB : ActionRepairVehiclePartCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.BASEBUILDING_REPAIR_MEDIUM);
