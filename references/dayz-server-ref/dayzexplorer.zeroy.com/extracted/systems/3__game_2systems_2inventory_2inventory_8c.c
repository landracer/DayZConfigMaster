// ======================================================================
// File: 3__game_2systems_2inventory_2inventory_8c_source.html
// Category: systems
// ======================================================================

//-------------------------------------------------------
enum InventoryCommandType
{
MOVE,
SYNC_MOVE,
HAND_EVENT,
SWAP,
FORCESWAP,
DESTROY,
REPLACE,
USER_RESERVATION_CANCEL
};
enum InventoryJunctureType
{
TAKE,
SWAP,
//LOAD, ///< load mag from ground
};
enum InventoryMode
{
PREDICTIVE,
LOCAL,
JUNCTURE,
SERVER,
};
enum InventoryValidationResult
{
FAILED,
JUNCTURE,
SUCCESS
};
enum InventoryValidationReason
{
UNKNOWN,
JUNCTURE_DENIED,
DROP_PREVENTED
};
class InventoryValidation
{
bool m_IsJuncture = false;
bool m_IsRemote = false;
InventoryValidationResult m_Result = InventoryValidationResult.FAILED;
InventoryValidationReason m_Reason = InventoryValidationReason.UNKNOWN;
InventoryMode m_Mode = InventoryMode.JUNCTURE;
bool IsAuthoritative()
{
return !m_IsJuncture && !m_IsRemote;
}
};
enum InventoryCheckContext
{
DEFAULT,
SYNC_CHECK,
}
enum FindInventoryReservationMode
{
LEGACY,
ITEM,
DST,
EQUAL,
};
class GameInventory
{
protected static int m_inventory_check_context = InventoryCheckContext.DEFAULT;
//-------------------------------------------------------
proto native EntityAI GetInventoryOwner();
#ifdef DEVELOPER
proto native void DumpInventoryDebug();
static proto native void DumpStaticInventoryDebug();
#endif
proto native bool HasEntityInInventory(notnull EntityAI item);
proto native bool EnumerateInventory(InventoryTraversalType tt, out array<EntityAI> items);
proto native int CountInventory();
proto native CargoBase GetCargo();
proto native CargoBase GetCargoFromIndex(int index);
proto native EntityAI CreateEntityInCargo(string typeName);
proto native EntityAI CreateEntityInCargoEx(string typeName, int idx, int row, int col, bool flip);
proto native bool HasEntityInCargo(notnull EntityAI e);
proto native bool HasEntityInCargoEx(notnull EntityAI e, int idx, int row, int col);
proto native bool CanAddEntityInCargo(notnull EntityAI e, bool flip);
proto native bool CanAddEntityInCargoEx(notnull EntityAI e, int idx, int row, int col, bool flip);
proto native bool CanAddEntityInCargoExLoc(InventoryLocation loc);
proto native bool TestAddEntityInCargoEx(notnull EntityAI e, int idx, int row, int col, bool flip, bool do_resevation_check, bool do_item_check, bool do_lock_check, bool do_occupancy_test, bool do_script_check, bool do_script_load_check);
proto native bool TestAddEntityInCargoExLoc(notnull InventoryLocation loc, bool do_resevation_check, bool do_item_check, bool do_lock_check, bool do_occupancy_test, bool do_script_check, bool do_script_load_check);
//proto native bool AddEntityInCargo (notnull EntityAI owner, EntityAI cargo);
//proto native bool AddEntityInCargoEx (notnull EntityAI owner, notnull EntityAI e, int idx, int row, int col);
proto native bool CanRemoveEntityInCargo(notnull EntityAI e);
proto native bool CanRemoveEntityInCargoEx(notnull EntityAI e, int idx, int row, int col);
proto native int GetSlotId(int index);
proto native int GetSlotIdCount();
proto native int GetAttachmentSlotId(int index);
proto native int GetAttachmentSlotsCount();
/*proto native*/bool HasAttachmentSlot(int slotId) //TODO - flip to code
{
int count = GetAttachmentSlotsCount();
for (int i = 0; i < count; i++)
{
if (GetAttachmentSlotId(i) == slotId)
return true;
}
return false;
}
proto native bool HasInventorySlot(int slotId);
proto native int AttachmentCount();
proto native EntityAI CreateAttachment(string typeName);
proto native EntityAI CreateAttachmentEx(string typeName, int slotId);
proto native EntityAI GetAttachmentFromIndex(int index);
proto native EntityAI FindAttachment(int slot);
proto native EntityAI FindAttachmentByName(string slotName);
proto native bool HasAttachment(notnull EntityAI e);
proto native bool HasAttachmentEx(notnull EntityAI e, int slot);
proto native bool CanAddAttachment(notnull EntityAI e);
proto native bool CanAddAttachmentEx(notnull EntityAI e, int slot);
proto native bool CanRemoveAttachment(EntityAI attachment);
proto native bool CanRemoveAttachmentEx(EntityAI attachment, int slot);
//proto native bool RemoveAttachment(EntityAI attachment);
//proto native bool RemoveAttachmentEx(EntityAI attachment, int slot);
proto native EntityAI FindPlaceholderForSlot(int slot);
proto native bool IsPlaceholderEntity(notnull Object e);
proto native bool GetCurrentInventoryLocation(out notnull InventoryLocation loc);
proto native bool FindFreeLocationFor(notnull EntityAI item, FindInventoryLocationType flags, out notnull InventoryLocation loc);
proto native bool FindFreeLocationForEx(notnull EntityAI item, FindInventoryLocationType flags, notnull InventoryLocation exclude, out notnull InventoryLocation loc);
proto native bool FindFirstFreeLocationForNewEntity(string item_type, FindInventoryLocationType flags, out notnull InventoryLocation loc);
proto native int FindFreeLocationsFor(notnull EntityAI item, FindInventoryLocationType flags, out notnull array<ref InventoryLocation> locs);
static proto native EntityAI LocationCreateEntity(notnull InventoryLocation inv_loc, string type, int iSetupFlags, int iRotation);
static proto native EntityAI LocationCreateLocalEntity(notnull InventoryLocation inv_loc, string type, int iSetupFlags, int iRotation);
static proto native bool LocationCanAddEntity(notnull InventoryLocation inv_loc);
//Added script check to LocationCanAddEntity
static bool LocationCanAddEntityEx(notnull InventoryLocation inv_loc)
{
return LocationCanAddEntity(inv_loc);
}
static proto native bool LocationTestAddEntity(notnull InventoryLocation inv_loc, bool do_resevation_check, bool do_item_check, bool do_lock_check, bool do_occupancy_test, bool do_script_check, bool do_script_load_check);
static proto native bool LocationCanRemoveEntity(notnull InventoryLocation inv_loc);
static proto native bool LocationCanMoveEntity(notnull InventoryLocation src, notnull InventoryLocation dst);
static int GetInventoryCheckContext()
{
return m_inventory_check_context;
}
static bool LocationCanMoveEntitySyncCheck(notnull InventoryLocation src, notnull InventoryLocation dst)
{
m_inventory_check_context = InventoryCheckContext.SYNC_CHECK;
bool result = LocationCanMoveEntity(src, dst);
m_inventory_check_context = InventoryCheckContext.DEFAULT;
return result;
}
static proto native EntityAI LocationGetEntity(notnull InventoryLocation inv_loc);
bool IsInCargo()
{
InventoryLocation lcn = new InventoryLocation();
GetCurrentInventoryLocation(lcn);
if (lcn.GetType() == InventoryLocationType.CARGO)
{
return true;
}
return false;
}
bool IsAttachment()
{
InventoryLocation lcn = new InventoryLocation();
GetCurrentInventoryLocation(lcn);
if (lcn.GetType() == InventoryLocationType.ATTACHMENT)
{
return true;
}
return false;
}
bool IsCargoInHiearchy()
{
InventoryLocation lcn = new InventoryLocation();
EntityAI ent = GetInventoryOwner();
while (ent)
{
if (ent.GetInventory().GetCurrentInventoryLocation(lcn) && lcn.IsValid())
{
if (lcn.GetType() == InventoryLocationType.CARGO || lcn.GetType() == InventoryLocationType.PROXYCARGO)
return true;
}
ent = ent.GetHierarchyParent();
}
return false;
}
bool AreChildrenAccessible()
{
EntityAI ent = GetInventoryOwner();
if (ent)
return ent.AreChildrenAccessible();
#ifdef DEVELOPER
ErrorEx("no inventory owner found!");
#endif
return true; //just in case inventoy without owner exists somewhere (shouldn&#39;t!)
}
bool GetCurrentAttachmentSlotInfo(out int slot_id, out string slot_name)
{
slot_id = InventorySlots.INVALID;
slot_name = "";
InventoryLocation lcn = new InventoryLocation();
GetCurrentInventoryLocation(lcn);
if (lcn.GetType() == InventoryLocationType.ATTACHMENT)
{
slot_id = lcn.GetSlot();
slot_name = InventorySlots.GetSlotName(slot_id);
return true;
}
return false;
}
static void OnServerInventoryCommandStatic(ParamsReadContext ctx)
{
int tmp = -1;
ctx.Read(tmp);
int type = -1;
if (!ctx.Read(type))
return;
switch (type)
{
case InventoryCommandType.SYNC_MOVE:
{
InventoryLocation src = new InventoryLocation();
InventoryLocation dst = new InventoryLocation();
src.ReadFromContext(ctx);
dst.ReadFromContext(ctx);
if (LogManager.IsSyncLogEnable()) syncDebugPrint("[syncinv] t=" + g_Game.GetTime() + "ms ServerInventoryCommand cmd=" + typename.EnumToString(InventoryCommandType, type) + " src=" + InventoryLocation.DumpToStringNullSafe(src) + " dst=" + InventoryLocation.DumpToStringNullSafe(dst));
if (!src.GetItem() || !dst.GetItem())
{
Error("[syncinv] ServerInventoryCommand (cmd=SYNC_MOVE) dropped, item not in bubble");
break; // not in bubble
}
LocationSyncMoveEntity(src, dst);
break;
}
case InventoryCommandType.HAND_EVENT:
{
HandEventBase e = HandEventBase.CreateHandEventFromContext(ctx);
if (LogManager.IsSyncLogEnable()) syncDebugPrint("[syncinv] t=" + g_Game.GetTime() + "ms ServerInventoryCommand cmd=" + typename.EnumToString(InventoryCommandType, type) + " event=" + e.DumpToString());
if (!e.GetSrcEntity())
{
Error("[syncinv] ServerInventoryCommand (cmd=HAND_EVENT) dropped, item not in bubble");
break; // not in bubble
}
e.m_Player.GetHumanInventory().ProcessHandEvent(e);
break;
}
case InventoryCommandType.FORCESWAP:
case InventoryCommandType.SWAP:
{
InventoryLocation src1 = new InventoryLocation();
InventoryLocation src2 = new InventoryLocation();
InventoryLocation dst1 = new InventoryLocation();
InventoryLocation dst2 = new InventoryLocation();
src1.ReadFromContext(ctx);
src2.ReadFromContext(ctx);
dst1.ReadFromContext(ctx);
dst2.ReadFromContext(ctx);
if (src1.IsValid() && src2.IsValid() && dst1.IsValid() && dst2.IsValid())
{
if (LogManager.IsSyncLogEnable()) syncDebugPrint("[syncinv] t=" + g_Game.GetTime() + "ms ServerInventoryCommand Swap src1=" + InventoryLocation.DumpToStringNullSafe(src1) + " src2=" + InventoryLocation.DumpToStringNullSafe(src2) + " dst1=" + InventoryLocation.DumpToStringNullSafe(dst1) + " dst2=" + InventoryLocation.DumpToStringNullSafe(dst2));
LocationSwap(src1, src2, dst1, dst2);
}
else
Error("ServerInventoryCommand - cannot swap, invalid location input: src1=" + InventoryLocation.DumpToStringNullSafe(src1) + " src2=" + InventoryLocation.DumpToStringNullSafe(src2) + " dst1=" + InventoryLocation.DumpToStringNullSafe(dst1) + " dst2=" + InventoryLocation.DumpToStringNullSafe(dst2));
break;
}
}
}
static proto native bool LocationAddEntity(notnull InventoryLocation inv_loc);
static proto native bool LocationRemoveEntity(notnull InventoryLocation inv_loc);
static proto native bool LocationMoveEntity(notnull InventoryLocation src_loc, notnull InventoryLocation dst_loc);
static proto native bool LocationSyncMoveEntity(notnull InventoryLocation src_loc, notnull InventoryLocation dst_loc);
static proto native bool LocationSwap(notnull InventoryLocation src1, notnull InventoryLocation src2, notnull InventoryLocation dst1, notnull InventoryLocation dst2);
static proto native bool ServerLocationMoveEntity(notnull EntityAI item, ParamsWriteContext ctx);
static proto native bool ServerLocationSyncMoveEntity(Man player, notnull EntityAI item, ParamsWriteContext ctx);
static proto native bool ServerLocationSwap(notnull InventoryLocation src1, notnull InventoryLocation src2, notnull InventoryLocation dst1, notnull InventoryLocation dst2, ParamsWriteContext ctx);
static proto native bool ServerHandEvent(notnull Man player, notnull EntityAI item, ParamsWriteContext ctx);
static proto native bool PrepareDropEntityPos(EntityAI owner, notnull EntityAI item, out vector mat[4], bool useValuesInMatrix = false, int conflictCheckDepth = -1);
static proto native bool TestDropEntityPos(EntityAI owner, notnull EntityAI item, out vector mat[4], bool useValuesInMatrix = false, int conflictCheckDepth = -1);
static proto native bool CanSwapEntities(notnull EntityAI item1, notnull EntityAI item2);
static bool CanSwapEntitiesEx(notnull EntityAI item1, notnull EntityAI item2)
{
int slot;
InventoryLocation il1 = new InventoryLocation();
InventoryLocation il2 = new InventoryLocation();
item2.GetInventory().GetCurrentInventoryLocation(il2);
slot = il2.GetSlot();
if (item1.CanBeSplit() && item1.GetQuantity() > item1.GetTargetQuantityMax(slot))
return false;
item1.GetInventory().GetCurrentInventoryLocation(il1);
slot = il1.GetSlot();
if (item2.CanBeSplit() && item2.GetQuantity() > item2.GetTargetQuantityMax(slot))
return false;
if (!item1.CanSwapEntities(item2, il2, il1) || !item2.CanSwapEntities(item1, il1, il2))
{
return false;
}
return CanSwapEntities(item1,item2);
}
static proto native bool CanForceSwapEntities(notnull EntityAI item1, InventoryLocation item1_dst, notnull EntityAI item2, out InventoryLocation item2_dst);
static bool CanForceSwapEntitiesEx(notnull EntityAI item1, InventoryLocation item1_dst, notnull EntityAI item2, out InventoryLocation item2_dst)
{
if (!CanForceSwapEntities(item1, item1_dst, item2, item2_dst) )
return false;
int slot;
InventoryLocation il = new InventoryLocation;
if (!item1.CanBeFSwaped())
return false;
if ( item1_dst == null)
{
item2.GetInventory().GetCurrentInventoryLocation(il);
slot = il.GetSlot();
}
else
{
slot = item1_dst.GetSlot();
}
if ( item1.GetQuantity() > item1.GetTargetQuantityMax(slot) )
return false;
if ( item2_dst == null)
{
item1.GetInventory().GetCurrentInventoryLocation(il);
slot = il.GetSlot();
}
else
{
slot = item2_dst.GetSlot();
}
if (!item1.CanSwapEntities(item2, item2_dst, item1_dst) || !item2.CanSwapEntities(item1, item1_dst, item2_dst))
{
return false;
}
if ( item2.GetQuantity() > item2.GetTargetQuantityMax(slot) )
return false;
return true;
}
proto native bool CanAddSwappedEntity(notnull InventoryLocation src1, notnull InventoryLocation src2, notnull InventoryLocation dst1, notnull InventoryLocation dst2);
const int c_InventoryReservationTimeoutMS = 5000;
const int c_InventoryReservationTimeoutShortMS = 3000;
static proto native bool AddInventoryReservation(EntityAI item, InventoryLocation dst, int timeout_ms);
bool AddInventoryReservationEx(EntityAI item, InventoryLocation dst, int timeout_ms)
{
if (g_Game.IsMultiplayer() && g_Game.IsServer() )
return true;
bool ret_val = AddInventoryReservation(item, dst, timeout_ms);
#ifdef ENABLE_LOGGING
if ( LogManager.IsInventoryReservationLogEnable() )
{
DayZPlayer player = g_Game.GetPlayer();
if ( player )
{
if (item)
Debug.InventoryReservationLog("Reservation result: " + ret_val + " - STS = " + player.GetSimulationTimeStamp() + " / " + item.ToString() + " / " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "AddInventoryReservation", player.ToString() );
else
Debug.InventoryReservationLog("Reservation result: " + ret_val + " - STS = " + player.GetSimulationTimeStamp() + " / null / " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "AddInventoryReservation", player.ToString() );
}
}
#endif
return ret_val;
}
static proto native bool ExtendInventoryReservation(EntityAI item, InventoryLocation dst, int timeout_ms);
bool ExtendInventoryReservationEx(EntityAI item, InventoryLocation dst, int timeout_ms)
{
if (g_Game.IsMultiplayer() && g_Game.IsServer() )
return true;
bool ret_val = ExtendInventoryReservation(item,dst,timeout_ms);
#ifdef ENABLE_LOGGING
if ( LogManager.IsInventoryReservationLogEnable() )
{
DayZPlayer player = g_Game.GetPlayer();
if ( player )
{
if (item)
Debug.InventoryReservationLog("Reservation result: " + ret_val + " - STS = " + player.GetSimulationTimeStamp() + " / " + item.ToString() + " / " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "ExtendInventoryReservation", player.ToString() );
else
Debug.InventoryReservationLog("Reservation result: " + ret_val + " - STS = " + player.GetSimulationTimeStamp() + " / null / " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "ExtendInventoryReservation", player.ToString() );
}
}
#endif
return ret_val;
}
static proto native bool ClearInventoryReservation(EntityAI item, InventoryLocation dst);
bool ClearInventoryReservationEx(EntityAI item, InventoryLocation dst)
{
if (g_Game.IsMultiplayer() && g_Game.IsServer() )
return true;
bool ret_val = ClearInventoryReservation(item,dst);
#ifdef ENABLE_LOGGING
if ( LogManager.IsInventoryReservationLogEnable() )
{
DayZPlayer player = g_Game.GetPlayer();
if ( player )
{
if (item)
Debug.InventoryReservationLog("Reservation cleared result: " + ret_val + " - STS = " + player.GetSimulationTimeStamp() + " / " + item.ToString() + " / " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "ClearInventoryReservation", player.ToString() );
else
Debug.InventoryReservationLog("Reservation cleared result: " + ret_val + " - STS = " + player.GetSimulationTimeStamp() + " / null / " + InventoryLocation.DumpToStringNullSafe(dst), "n/a" , "n/a", "ClearInventoryReservation", player.ToString() );
}
}
#endif
return ret_val;
}
static proto native bool HasInventoryReservation(EntityAI item, InventoryLocation dst);
static proto native bool HasInventoryReservationCanAdd(EntityAI item, InventoryLocation dst);
static proto native bool HasInventoryReservationEx(EntityAI item, InventoryLocation dst, FindInventoryReservationMode itemMode, FindInventoryReservationMode parentMode);
static proto native bool GetInventoryReservationCount(EntityAI item, InventoryLocation dst);
proto native int GetAnyInventoryReservationCount();
proto native bool CanLockInventoryWithKey(notnull EntityAI key);
proto native bool CanUnlockInventoryWithKey(notnull EntityAI key);
proto native void LockInventoryWithKey(notnull EntityAI key);
proto native void UnlockInventoryWithKey(notnull EntityAI key);
proto native bool HasKeys();
proto native void LockInventory(int lockType);
proto native void UnlockInventory(int lockType);
proto native int GetScriptLockCount();
proto native bool IsInventoryUnlocked();
proto native bool IsInventoryLocked();
proto native bool IsInventoryLockedForLockType(int lockType);
proto native bool SetSlotLock(int slot, bool locked);
proto native bool GetSlotLock(int slot);
const float c_MaxItemDistanceRadius = 2.5;
static proto native bool CheckRequestSrc(notnull Man requestingPlayer, notnull InventoryLocation src, float radius);
static proto native bool CheckDropRequest(notnull Man requestingPlayer, notnull InventoryLocation src, float radius);
static proto native bool CheckTakeItemRequest(notnull Man requestingPlayer, notnull InventoryLocation src, notnull InventoryLocation dst, float radius);
static proto native bool CheckMoveToDstRequest(notnull Man requestingPlayer, notnull InventoryLocation src, notnull InventoryLocation dst, float radius);
static proto native bool CheckSwapItemsRequest(notnull Man requestingPlayer, notnull InventoryLocation src1, notnull InventoryLocation src2, notnull InventoryLocation dst1, notnull InventoryLocation dst2, float radius);
static proto native bool CheckManipulatedObjectsDistances(notnull EntityAI e0, notnull EntityAI e1, float radius);
void Init()
{
GetInventoryOwner().OnInventoryInit();
}
bool OnStoreLoad(ParamsReadContext ctx, int version)
{
return true;
}
void OnAfterStoreLoad();
void OnStoreSave(ParamsWriteContext ctx);
void EEInit()
{
InventoryLocation src = new InventoryLocation;
if (GetCurrentInventoryLocation(src))
{
if (src.GetType() == InventoryLocationType.HANDS)
{
Man man = Man.Cast(src.GetParent());
if (man)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("Inventory::EEInit - Man=" + man + " item=" + this);
man.GetHumanInventory().OnEntityInHandsCreated(src);
}
}
}
}
void EEDelete(EntityAI parent)
{
EntityAI item = GetInventoryOwner();
Man player = item.GetHierarchyRootPlayer();
if (player)
player.GetInventory().ClearInventoryReservationEx(item, null);
}
EntityAI CreateInInventory(string type)
{
InventoryLocation loc = new InventoryLocation();
if (FindFirstFreeLocationForNewEntity(type, FindInventoryLocationType.CARGO | FindInventoryLocationType.ATTACHMENT, loc))
{
switch (loc.GetType())
{
case InventoryLocationType.ATTACHMENT:
return loc.GetParent().GetInventory().CreateAttachmentEx(type, loc.GetSlot());
case InventoryLocationType.CARGO:
return loc.GetParent().GetInventory().CreateEntityInCargoEx(type, loc.GetIdx(), loc.GetRow(), loc.GetCol(), loc.GetFlip());
default:
Error("CreateInInventory: unknown location for item");
break;
}
}
return null;
}
bool CanAddEntityToInventory(notnull EntityAI item, int flag = FindInventoryLocationType.ANY)
{
InventoryLocation il = new InventoryLocation();
bool result = FindFreeLocationFor(item, flag, il);
return result;
}
bool AddEntityToInventory(notnull EntityAI item)
{
InventoryLocation il = new InventoryLocation();
bool result = FindFreeLocationFor(item, FindInventoryLocationType.ANY, il);
if (result)
return LocationAddEntity(il);
return result;
}
bool CanRemoveEntity()
{
InventoryLocation il = new InventoryLocation;
if (GetCurrentInventoryLocation(il))
return LocationCanRemoveEntity(il);
return false;
}
// Script version of CanAddEntity* methods based on InventoryLocation
bool CanAddEntityInto(notnull EntityAI item, FindInventoryLocationType flags = FindInventoryLocationType.ANY)
{
InventoryLocation loc = new InventoryLocation();
return FindFreeLocationFor(item, flags, loc) && !item.IsHologram();
}
bool CanAddEntityIntoInventory(notnull EntityAI item)
{
return CanAddEntityInto(item, FindInventoryLocationType.ANY_CARGO | FindInventoryLocationType.ATTACHMENT);
}
bool CanAddEntityIntoHands(notnull EntityAI item)
{
return CanAddEntityInto(item, FindInventoryLocationType.HANDS);
}
bool OnInputUserDataProcess(ParamsReadContext ctx);
bool OnInventoryJunctureFromServer(ParamsReadContext ctx);
void OnInventoryJunctureFailureFromServer(ParamsReadContext ctx);
void OnServerInventoryCommand(ParamsReadContext ctx);
void OnInventoryFailure(InventoryCommandType type, InventoryValidationReason reason, InventoryLocation src, InventoryLocation dst);
bool TakeEntityToInventory(InventoryMode mode, FindInventoryLocationType flags, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
if (FindFreeLocationFor(item, flags, dst))
return TakeToDst(mode, src, dst);
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " Warning - no room for item");
return false;
}
Error("[inv] I::Take2Inv(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " Error - src has no inventory location");
return false;
}
bool TakeEntityToTargetInventory(InventoryMode mode, notnull EntityAI target, FindInventoryLocationType flags, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Target(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
if (target.GetInventory().FindFreeLocationFor(item, flags, dst))
return TakeToDst(mode, src, dst);
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Target(" + typename.EnumToString(InventoryMode, mode) + ") target=" + target + " item=" + item + " Warning - no room for item in target");
return false;
}
Error("[inv] I::Take2Target(" + typename.EnumToString(InventoryMode, mode) + ") target=" + target + " item=" + item + " Error - src has no inventory location");
return false;
}
bool TakeToDst (InventoryMode mode, notnull InventoryLocation src, notnull InventoryLocation dst)
{
bool ret;
switch (mode)
{
case InventoryMode.SERVER:
ret = LocationSyncMoveEntity(src, dst);
if (ret && dst.IsValid())
InventoryInputUserData.SendServerMove(null, InventoryCommandType.SYNC_MOVE, src, dst);
return ret;
case InventoryMode.LOCAL:
ret = LocationSyncMoveEntity(src, dst);
return ret;
default:
return false;
}
return false;
}
bool TakeEntityToCargo(InventoryMode mode, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
return TakeEntityToInventory(mode, FindInventoryLocationType.CARGO, item);
}
bool TakeEntityToTargetCargo(InventoryMode mode, notnull EntityAI target, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2TargetCgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + "to cargo of target=" + target);
return TakeEntityToTargetInventory(mode, target, FindInventoryLocationType.CARGO, item);
}
bool TakeEntityToCargoEx(InventoryMode mode, notnull EntityAI item, int idx, int row, int col)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " row=" + row + " col=" + col);
InventoryLocation src = new InventoryLocation();
GameInventory itemInventory = item.GetInventory();
if (itemInventory.GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
dst.SetCargo(GetInventoryOwner(), item, idx, row, col, itemInventory.GetFlipCargo());
return TakeToDst(mode, src, dst);
}
Error("[inv] I::Take2Cgo(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " row=" + row + " col=" + col + " Error - src has no inventory location");
return false;
}
bool TakeEntityToTargetCargoEx(InventoryMode mode, notnull CargoBase cargo, notnull EntityAI item, int row, int col)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2TargetCgoEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + "to cargo of target=" + cargo.GetCargoOwner() + " row=" + row + " col=" + col);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
dst.SetCargoAuto(cargo, item, row, col, item.GetInventory().GetFlipCargo());
return TakeToDst(mode, src, dst);
}
Error("[inv] I::Take2TargetCgoEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + "to cargo of target=" + cargo.GetCargoOwner() + " row=" + row + " col=" + col);
return false;
}
bool TakeEntityAsAttachmentEx(InventoryMode mode, notnull EntityAI item, int slot)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " slot=" + slot);
return TakeEntityAsTargetAttachmentEx(mode, GetInventoryOwner(), item, slot);
}
bool TakeEntityAsTargetAttachmentEx(InventoryMode mode, notnull EntityAI target, notnull EntityAI item, int slot)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2TargetAttEx(" + typename.EnumToString(InventoryMode, mode) + ") as ATT of target=" + target + " item=" + item + " slot=" + slot);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
EntityAI att = target.GetInventory().FindAttachment(slot);
if (att)
{
if (mode == InventoryMode.SERVER)
{
att.CombineItemsEx(item, true);
}
else
{
att.CombineItemsClient(item, true);
}
return true;
}
else if (item.CanBeSplit() && item.GetTargetQuantityMax(slot) < item.GetQuantity())
{
if (mode == InventoryMode.SERVER)
{
item.SplitIntoStackMaxEx(target, slot);
}
else
{
item.SplitIntoStackMaxClient(target,slot);
}
return true;
}
else
{
InventoryLocation dst = new InventoryLocation();
dst.SetAttachment(target, item, slot);
return TakeToDst(mode, src, dst);
}
}
Error("[inv] I::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item + " Error - src has no inventory location");
return false;
}
bool TakeEntityAsAttachment(InventoryMode mode, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2Att(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
return TakeEntityToInventory(mode, FindInventoryLocationType.ATTACHMENT, item);
}
bool TakeEntityAsTargetAttachment(InventoryMode mode, notnull EntityAI target, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Take2AttEx(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
return TakeEntityToTargetInventory(mode, target, FindInventoryLocationType.ATTACHMENT, item);
}
static bool MakeDstForSwap(notnull InventoryLocation src1, notnull InventoryLocation src2, out InventoryLocation dst1, out InventoryLocation dst2)
{
if (dst1 == null)
dst1 = new InventoryLocation();
dst1.Copy(src1);
dst1.CopyLocationFrom(src2, false);
dst1.SetFlip(src1.GetItem().GetInventory().GetFlipCargo()); // update flip flag from inventory item
if (dst2 == null)
dst2 = new InventoryLocation();
dst2.Copy(src2);
dst2.CopyLocationFrom(src1, false);
dst2.SetFlip(src2.GetItem().GetInventory().GetFlipCargo()); // update flip flag from inventory item
return true;
}
static bool MakeSrcAndDstForSwap(notnull EntityAI item1, notnull EntityAI item2, out InventoryLocation src1, out InventoryLocation src2, out InventoryLocation dst1, out InventoryLocation dst2)
{
if (src1 == null)
src1 = new InventoryLocation();
if (src2 == null)
src2 = new InventoryLocation();
if (item1.GetInventory().GetCurrentInventoryLocation(src1) && item2.GetInventory().GetCurrentInventoryLocation(src2))
return MakeDstForSwap(src1, src2, dst1, dst2);
return false;
}
static bool MakeSrcAndDstForForceSwap(notnull EntityAI item1, notnull EntityAI item2, out InventoryLocation src1, out InventoryLocation src2, out InventoryLocation dst1, notnull InventoryLocation dst2)
{
if (src1 == null)
src1 = new InventoryLocation();
if (src2 == null)
src2 = new InventoryLocation();
if (item1.GetInventory().GetCurrentInventoryLocation(src1) && item2.GetInventory().GetCurrentInventoryLocation(src2))
{
// src1 -> dst_of(src2)
if (dst1 == null)
dst1 = new InventoryLocation();
dst1.Copy(src1);
dst1.CopyLocationFrom(src2, false);
dst1.SetFlip(dst1.GetItem().GetInventory().GetFlipCargo());
// src2 -> dst2 from user
return true;
}
return false;
}
bool SwapEntities(InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2)
{
return false;
}
bool ForceSwapEntities(InventoryMode mode, notnull EntityAI item1, notnull EntityAI item2, notnull InventoryLocation item2_dst)
{
return false;
}
static bool SetGroundPosByOwner(EntityAI owner, notnull EntityAI item, out InventoryLocation ground)
{
vector m4[4];
Math3D.MatrixIdentity4(m4);
bool success = GameInventory.PrepareDropEntityPos(owner, item, m4, false, GameConstants.INVENTORY_ENTITY_DROP_OVERLAP_DEPTH);
ground.SetGround(item, m4);
return success;
}
bool DropEntity(InventoryMode mode, EntityAI owner, notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Drop(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
if (!SetGroundPosByOwner(owner, item, dst))
{
OnInventoryFailure(InventoryCommandType.SYNC_MOVE, InventoryValidationReason.DROP_PREVENTED, src, dst);
return false;
}
return TakeToDst(mode, src, dst);
}
Error("DropEntity - No inventory location");
return false;
}
static bool SetGroundPosByTransform(EntityAI owner, notnull EntityAI item, out InventoryLocation ground, vector transform[4])
{
bool success = GameInventory.PrepareDropEntityPos(owner, item, transform, true, GameConstants.INVENTORY_ENTITY_DROP_OVERLAP_DEPTH);
ground.SetGround(item, transform);
return success;
}
bool DropEntityWithTransform(InventoryMode mode, EntityAI owner, notnull EntityAI item, vector transform[4])
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Drop(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
SetGroundPosByTransform(owner, item, dst, transform);
return TakeToDst(mode, src, dst);
}
Error("DropEntityWithTransform - No inventory location");
return false;
}
static void SetGroundPosByOwnerBounds(EntityAI owner, notnull EntityAI item, out InventoryLocation ground, vector halfExtents, float angle, float cosAngle, float sinAngle)
{
vector m4[4];
owner.GetTransform(m4);
vector randomPos = Vector(Math.RandomFloat(-halfExtents[0], halfExtents[0]), 0, Math.RandomFloat(-halfExtents[2], halfExtents[2]));
randomPos = vector.RotateAroundZero(randomPos, vector.Up, cosAngle, sinAngle);
float dist = randomPos[0] * m4[1][0] + randomPos[1] * m4[1][1] + randomPos[2] * m4[1][2];
m4[3][0] = m4[3][0] + randomPos[0];
m4[3][1] = m4[3][1] - dist + halfExtents[1];
m4[3][2] = m4[3][2] + randomPos[2];
item.PlaceOnSurfaceRotated(m4, Vector(m4[3][0], m4[3][1], m4[3][2]));
ground.SetGround(item, m4);
}
bool DropEntityInBounds(InventoryMode mode, EntityAI owner, notnull EntityAI item, vector halfExtents, float angle, float cosAngle, float sinAngle)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::Drop(" + typename.EnumToString(InventoryMode, mode) + ") item=" + item);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
InventoryLocation dst = new InventoryLocation();
SetGroundPosByOwnerBounds(owner, item, dst, halfExtents, angle, cosAngle, sinAngle);
return TakeToDst(mode, src, dst);
}
Error("DropEntityInBounds - No inventory location");
return false;
}
bool LocalDestroyEntity(notnull EntityAI item)
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::LocalDestroyEntity inv item=" + item);
InventoryLocation src = new InventoryLocation();
if (item.GetInventory().GetCurrentInventoryLocation(src))
{
if (src.GetType() == InventoryLocationType.HANDS)
Error("[inv] Source location == HANDS, player has to handle this");
g_Game.ObjectDelete(src.GetItem());
return true;
}
Error("LocalDestroyEntity: No inventory location");
return false;
}
bool ReplaceItemWithNew(InventoryMode mode, ReplaceItemWithNewLambdaBase lambda)
{
InventoryLocation src = new InventoryLocation();
if (lambda.m_OldItem.GetInventory().GetCurrentInventoryLocation(src))
{
if (LogManager.IsInventoryMoveLogEnable()) inventoryDebugPrint("[inv] I::ReplaceItemWithNew executing lambda=" + lambda + "on old_item=" + lambda.m_OldItem);
lambda.Execute();
return true;
}
Error("[inv] I::ReplaceItemWithNew - no inventory location");
return false;
}
proto native bool GetFlipCargo();
proto native void SetFlipCargo(bool flip);
proto native void FlipCargo();
proto native void ResetFlipCargo();
}