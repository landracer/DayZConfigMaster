// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionlockeddoors.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionLockedDoors: ActionInteractBase
{
    void ActionLockedDoors()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        //m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#open&quot;;
