// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actionclosecardoors.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionCloseCarDoors: ActionCarDoors
{
    void ActionCloseCarDoors()
    {   
        m_IsOpening     = false;
        
        FillCommandUIDPerCrewIdx(DayZPlayerConstants.CMD_ACTIONMOD_DRIVER_DOOR_CLOSE, DayZPlayerConstants.CMD_ACTIONMOD_CODRIVER_DOORCLOSE);
        m_Text = &quot;#close&quot;;
