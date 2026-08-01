// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakearrowtohands.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTakeArrowToHands: ActionInteractBase
{
    void ActionTakeArrowToHands()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS;
        m_Text              = &quot;#take_to_hands&quot;;
