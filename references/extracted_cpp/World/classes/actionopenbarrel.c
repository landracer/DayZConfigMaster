// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionopenbarrel.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionOpenBarrel: ActionInteractBase
{
    void ActionOpenBarrel()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#open&quot;;
