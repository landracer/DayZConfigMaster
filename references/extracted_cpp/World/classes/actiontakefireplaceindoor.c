// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakefireplaceindoor.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionTakeFireplaceIndoor: ActionInteractBase
{
    string m_NewItemTypeName = &quot;Fireplace&quot;;
 
    void ActionTakeFireplaceIndoor()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
        m_Text = &quot;#take_fireplace&quot;;
