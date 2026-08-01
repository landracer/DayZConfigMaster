// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actionopencardoorsoutside.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionOpenCarDoorsOutside: ActionCarDoorsOutside
{
    void ActionOpenCarDoorsOutside()
    {
        m_IsOpening     = true;
        
        FillCommandUIDPerCrewIdx(DayZPlayerConstants.CMD_ACTIONMOD_DRIVER_DOOR_OPEN, DayZPlayerConstants.CMD_ACTIONMOD_CODRIVER_DOOROPEN);
        m_Text = &quot;#open&quot;;
