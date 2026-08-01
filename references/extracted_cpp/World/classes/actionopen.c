// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionopen.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionOpen: ActionSingleUseBase
{
    void ActionOpen()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENITEM_ONCE;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_OPENITEM_ONCE;
        //m_Animation = &quot;open&quot;;
        m_Text = &quot;#open&quot;;
