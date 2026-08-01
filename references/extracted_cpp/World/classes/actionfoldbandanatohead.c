// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionfoldbandanatohead.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFoldBandanaToHead : ActionFoldEntityToSlot
{
    void ActionFoldBandanaToHead ()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
        m_SlotID = InventorySlots.HEADGEAR;
