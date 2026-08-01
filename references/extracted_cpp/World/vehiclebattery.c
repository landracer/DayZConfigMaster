// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/vehiclebattery.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class VehicleBattery : ItemBase
{
    override bool CanPutAsAttachment(EntityAI parent)
    {
        if (!super.CanPutAsAttachment(parent)) 
            return false;
        
        if (parent.IsInherited(BatteryCharger))
        {
            BatteryCharger charger = BatteryCharger.Cast(parent);
            return charger.CanReceiveAttachment(this, InventorySlots.INVALID);
        }
        
        if (GetCompEM().GetPluggedDevice())
            return false;
        
        return true;
