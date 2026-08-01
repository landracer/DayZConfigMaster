// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakefireplacefrombarrel.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionTakeFireplaceFromBarrel: ActionInteractBase
{
    void ActionTakeFireplaceFromBarrel()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#take_fireplace&quot;;
