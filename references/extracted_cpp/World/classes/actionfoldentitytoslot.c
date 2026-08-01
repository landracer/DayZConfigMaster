// Source: F:/Games/Dayz/scripts/4_world/classes/useractionscomponent/actions/singleuse/actionfoldentitytoslot.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class ActionFoldEntityToSlot : ActionSingleUseBase
{
    int m_SlotID = InventorySlots.INVALID;
 
    override void CreateConditionComponents ()
    {
        m_ConditionItem = new CCINonRuined;
        m_ConditionTarget = new CCTNone;
