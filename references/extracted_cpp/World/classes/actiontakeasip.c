// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiontakeasip.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionTakeASip: ActionTakeABite
{
    void ActionTakeASip()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
        m_Text = &quot;#take_a_sip&quot;;
