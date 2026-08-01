// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionturnonwhileonground.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOnWhileOnGround: ActionInteractBase
{
    void ActionTurnOnWhileOnGround()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#switch_on&quot;;
