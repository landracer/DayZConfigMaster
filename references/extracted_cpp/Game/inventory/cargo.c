// Source: F:/Games/Dayz/scripts/3_game/systems/inventory/cargo.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class CargoBase : Managed
{
    proto native EntityAI GetCargoOwner ();

    proto native int GetOwnerCargoIndex ();
    
    proto native bool IsProxyCargo ();
    
    proto native int GetItemCount ();
    proto native EntityAI GetItem (int index);

    proto native int GetWidth ();
    proto native int GetHeight ();
    proto bool GetItemRowCol (int index, out int row, out int col);
    proto bool GetItemSize (int index, out int w, out int h);

    proto native int FindEntityInCargo (notnull EntityAI e);
 
    private void CargoBase ();
    private void ~CargoBase ();
    
    bool CanReceiveItemIntoCargo (EntityAI item) { return true; }

    bool CanSwapItemInCargo (EntityAI child_entity, EntityAI new_entity) { return true; }
    
    proto native int GetUserReservedLocationCount ();
    proto bool GetUserReservedLocation (int index, out EntityAI eai, out int row, out int col, out int w, out int h, out int flp);
    proto native void SetUserReservedLocation (notnull EntityAI eai);
    proto native void ClearUserReservedLocation (notnull EntityAI eai);
