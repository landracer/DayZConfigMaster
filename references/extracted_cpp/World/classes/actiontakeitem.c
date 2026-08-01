// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/interact/actiontakeitem.c Source File
// Extracted from DayZ Code Explorer by Zeroy

/*class ActionTakeItemReciveData : ActionReciveData
{
    ref InventoryLocation       m_InventoryLocation;
}*/
 
class ActionTakeItem: ActionInteractBase
{
    string m_ItemName = &quot;&quot;;
 
    void ActionTakeItem()
    {
        m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_INVENTORY;
        m_CommandUIDProne   = DayZPlayerConstants.CMD_ACTIONFB_PICKUP_INVENTORY;
        m_Text = &quot;#take&quot;;
