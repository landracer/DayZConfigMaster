// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionturnoffwhileonground.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOffWhileOnGround: ActionInteractBase
{   
    void ActionTurnOffWhileOnGround()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#switch_off&quot;;
