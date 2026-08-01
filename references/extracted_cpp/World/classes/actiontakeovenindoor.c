// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakeovenindoor.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionTakeOvenIndoor: ActionInteractBase
{
    string m_NewItemTypeName = &quot;Fireplace&quot;;
 
    void ActionTakeOvenIndoor()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#take_fireplace&quot;;
