// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionopenbarrelholes.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionOpenBarrelHoles: ActionInteractBase
{
    void ActionOpenBarrelHoles()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_Text = &quot;#open&quot;;
