// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/continuous/vehicles/actioncarhorn.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCarHornBase
{
    static bool ActionCondition(PlayerBase player)
    {
        HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
 
        if (vehCommand)
        {
            Transport trans = vehCommand.GetTransport();
            if (trans)
            {
                if (!PlayerIsDriver(trans, player))
                {
                    return false;
                }
                
                CarScript car;
                if (Class.CastTo(car, trans))
                {
                    return BatteryIsVital(car.GetBattery());
                }
            }
        }
        
        return false;
