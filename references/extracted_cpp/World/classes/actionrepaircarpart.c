// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionrepaircarpart.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class RepairCarPartActionReciveData : RepairVehiclePartActionReciveData
{}
 
class RepairCarPartActionData : RepairVehiclePartActionData
{}
 
class ActionRepairCarPartCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousRepeat(UATimeSpent.BASEBUILDING_REPAIR_FAST);
