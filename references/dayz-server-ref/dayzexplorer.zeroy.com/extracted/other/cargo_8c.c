// ======================================================================
// File: cargo_8c_source.html
// Category: other
// ======================================================================

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
};
#ifdef PLATFORM_CONSOLE
class CargoList : CargoBase
{
proto native int GetMaxWeight ();
proto native int GetTotalWeight (EntityAI item);
proto native bool CanFitItemIntoCargo (EntityAI cargo);
override bool CanReceiveItemIntoCargo (EntityAI item)
{
return true;
//return CanFitItemIntoCargo(item);
}
proto native bool CanFitSwappedItemInCargo (EntityAI child_entity, EntityAI new_entity);
override bool CanSwapItemInCargo (EntityAI child_entity, EntityAI new_entity)
{
return CanFitSwappedItemInCargo(child_entity, new_entity);
}
};
#else
#ifdef SERVER_FOR_CONSOLE
class CargoList : CargoBase
{
proto native int GetMaxWeight ();
proto native int GetTotalWeight (EntityAI item);
proto native bool CanFitItemIntoCargo (EntityAI cargo);
override bool CanReceiveItemIntoCargo (EntityAI item)
{
return true;
//return CanFitItemIntoCargo(item);
}
proto native bool CanFitSwappedItemInCargo (EntityAI child_entity, EntityAI new_entity);
override bool CanSwapItemInCargo (EntityAI child_entity, EntityAI new_entity)
{
return CanFitSwappedItemInCargo(child_entity, new_entity);
}
};
#else
class CargoGrid : CargoBase
{
proto native EntityAI FindEntityInCargoOn (int row, int col);
};
#endif
#endif