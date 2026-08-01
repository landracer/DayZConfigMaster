// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/humaninventory.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class HumanInventory : GameInventory
{   
    int m_syncClearUserReservationindex = -1;
    //int m_UserReservationToClear = -1;
    proto native EntityAI GetEntityInHands();

    proto native bool CanAddEntityInHands(EntityAI e);

    proto native bool TestAddEntityInHands(EntityAI e, bool do_resevation_check, bool do_item_check, bool do_lock_check, bool do_occupancy_test, bool do_script_check);

    proto native bool CanRemoveEntityInHands();
 
    proto native bool CanOpenInventory();

    proto native EntityAI CreateInHands(string typeName);
 
    proto native int GetUserReservedLocationCount();
    proto native int FindUserReservedLocationIndex(notnull EntityAI e);
    proto native int FindCollidingUserReservedLocationIndex(notnull EntityAI e, notnull InventoryLocation dst);
    proto native void GetUserReservedLocation(int index, out notnull InventoryLocation dst);
    proto native int FindFirstUserReservedLocationIndexForContainer(notnull EntityAI e);
    
    proto native void SetUserReservedLocation(notnull EntityAI eai, notnull InventoryLocation dst);
    proto native void ClearUserReservedLocation(notnull EntityAI eai);
    proto native bool ClearUserReservedLocationAtIndex(int index);
    proto native void ClearUserReservedLocationForContainer(notnull EntityAI eai);
    proto native bool GetDebugFlag();

    override EntityAI CreateInInventory(string type)
    {
        EntityAI newEntity = super.CreateInInventory(type);
        if (newEntity == null)
            newEntity = CreateInHands(type);
        return newEntity;
