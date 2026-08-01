// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionpulloutplug.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionPullOutPlug : ActionInteractBase
{
    void ActionPullOutPlug()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
 
        m_Text = &quot;#pull_out_plug&quot;;
