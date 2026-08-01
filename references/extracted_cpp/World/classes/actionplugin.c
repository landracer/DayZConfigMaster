// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionplugin.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPlugIn: ActionSingleUseBase
{
    void ActionPlugIn()
    {
        m_CommandUID    = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_Text          = &quot;#plug_in&quot;;
