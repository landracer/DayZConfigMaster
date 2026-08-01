// Source: F:/Games/Dayz/scripts/4_world/entities/dayzplayerimplementvehicle.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class GearChangeActionCallback : HumanCommandActionCallback
{   
    // Left for backwards compatibility, but never ever store HumanCommandVehicle in a variable used later
    HumanCommandVehicle m_hcv;
    
    void SetVehicleCommand(HumanCommandVehicle hcv)
    {
        //m_hcv = hcv;
        hcv.SetClutchState(true);
