// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/vehicles/actionswitchlights.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionSwitchLights: ActionInteractBase
{
    void ActionSwitchLights()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_HEADLIGHT;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text              = &quot;#switch_lights&quot;;
