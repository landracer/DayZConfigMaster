// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/actionupgradetorchfromgaspump.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionUpgradeTorchFromGasPumpCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.ADD_FUEL_TO_TORCH);
