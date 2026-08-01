// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionfoldbandanatomask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFoldBandanaToMask : ActionFoldEntityToSlot
{
    void ActionFoldBandanaToMask ()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_SlotID = InventorySlots.MASK;
