// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakehybridattachment.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class ActionTakeHybridAttachment: ActionInteractBase
{
    string m_ItemName = &quot;&quot;;
 
    void ActionTakeHybridAttachment()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_PICKUP_INVENTORY;
        m_Text = &quot;#take&quot;;
