// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionoperatepanelpowerstation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionOperatePanelPowerStation : ActionOperatePanel
{
    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        if (!target) 
            return false;
        
        Land_WarheadStorage_PowerStation powerStation = Land_WarheadStorage_PowerStation.Cast(target.GetObject());
        if (!powerStation)
            return false;
    
        int leverComponent = target.GetComponentIndex();
        string leverName = powerStation.GetActionComponentName(leverComponent);
        int leverIndex = powerStation.GetLeverIndexByComponentName(leverName);
 
        return (leverIndex != 0);   
