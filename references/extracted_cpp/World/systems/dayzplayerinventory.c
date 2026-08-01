// Source: F:/Games/Dayz/scripts/4_world/systems/inventory/dayzplayerinventory.c Source File
// Extracted from DayZ Code Explorer by Zeroy

//Post event containers
class DeferredEvent
{
    InventoryMode m_mode;
    bool ReserveInventory(HumanInventory inventory){return true;}
    void ClearInventoryReservation(HumanInventory inventory){}
