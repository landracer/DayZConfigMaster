// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakearrow.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTakeArrow: ActionTakeItem
{
    void ActionTakeArrow()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_Text              = &quot;#take&quot;;
