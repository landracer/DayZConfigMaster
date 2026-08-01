// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actiontogglenvmode.c Source File
// Extracted from DayZ Code Explorer by Zeroy

 
class ActionToggleNVMode: ActionSingleUseBase
{
    void ActionToggleNVMode()
    {
        //m_CallbackClass = ActionTurnOnTransmitterCB;
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
        m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_ITEM_ON;
        m_Text = &quot;#switch_mode&quot;;
