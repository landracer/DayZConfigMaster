// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actionturnofftransmitteronground.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTurnOffTransmitterOnGround: ActionInteractBase
{
    void ActionTurnOffTransmitterOnGround()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#turn_off&quot;;
