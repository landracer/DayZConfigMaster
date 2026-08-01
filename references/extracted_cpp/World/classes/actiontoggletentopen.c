// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontoggletentopen.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionToggleTentOpen: ActionInteractBase
{
    void ActionToggleTentOpen()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text = &quot;#toggle_opening&quot;;
