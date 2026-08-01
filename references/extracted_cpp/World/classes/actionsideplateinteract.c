// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actionsideplateinteract.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionSidePlateInteract: ActionAnimateCarSelection
{
    void ActionSidePlateInteract()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask    = DayZPlayerConstants.STANCEMASK_ALL;
