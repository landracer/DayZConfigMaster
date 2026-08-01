// ======================================================================
// File: inventorylocation_8c_source.html
// Category: other
// ======================================================================

//@NOTE: DO NOT EDIT! enum values are overwritten from c++
enum InventoryLocationType
{
UNKNOWN,
GROUND,
ATTACHMENT,
CARGO,
HANDS,
PROXYCARGO,
VEHICLE,
TEMP,
};
//@NOTE: DO NOT EDIT! enum values are overwritten from c++
enum FindInventoryLocationType
{
ATTACHMENT,
CARGO,
HANDS,
PROXYCARGO,
ANY_CARGO,
ANY,
NO_SLOT_AUTO_ASSIGN
};
class InventoryLocation
{
proto native bool IsValid();
proto native int GetType();
proto native EntityAI GetParent();
proto native EntityAI GetItem();
proto native int GetSlot();
proto native int GetIdx();
proto native int GetRow();
proto native int GetCol();
proto native bool GetFlip();
proto native vector GetPos();
proto native void GetDir(out float dir[4]);
proto native void SetGround(EntityAI e, vector mat[4]);
proto native void SetGroundEx(EntityAI e, vector pos, float dir[4]);
proto native void SetAttachment(notnull EntityAI parent, EntityAI e, int slotId);
proto native void SetCargoAuto(notnull CargoBase cargo, EntityAI e, int row, int col, bool flip);
proto native void SetCargo(notnull EntityAI parent, EntityAI e, int idx, int row, int col, bool flip);
proto native void SetProxyCargo(notnull EntityAI parent, EntityAI e, int idx, int row, int col, bool flip);
proto native void SetHands(notnull EntityAI parent, EntityAI e);
proto native void SetVehicle(notnull EntityAI parent, EntityAI e, int idx);
proto native void SetTemporary(notnull EntityAI parent, EntityAI e);
proto native void SetParent(notnull EntityAI parent);
proto native void SetItem(notnull EntityAI item);
// direct set methods
proto native void SetSlot(int slotId);
proto native void SetIndex(int idx);
proto native void SetRow(int row);
proto native void SetCol(int col);
proto native void SetFlip(bool flip);
proto native void Reset();
proto native bool CompareLocationOnly(notnull InventoryLocation other);
proto native bool CollidesWith(notnull InventoryLocation rhs);
proto native InventoryLocation Copy(notnull InventoryLocation rhs);
proto native InventoryLocation CopyLocationFrom(notnull InventoryLocation rhs, bool copyFlip);
static string DumpToStringNullSafe(InventoryLocation loc)
{
if (loc)
return loc.DumpToString();
return "{ null }";
}
string DumpToString()
{
string res = "{ type=" + typename.EnumToString(InventoryLocationType, GetType());
switch (GetType())
{
case InventoryLocationType.UNKNOWN:
{
break;
}
case InventoryLocationType.GROUND:
{
res = res + " item=" + Object.GetDebugName(GetItem());
vector pos = GetPos();
float dir[4];
GetDir(dir);
res = res + " pos=(" + pos[0] + ", " + pos[1] + ", " + pos[2] + ")";
res = res + " dir=(" + dir[0] + ", " + dir[1] + ", " + dir[2] + ", " + dir[3] + ")";
break;
}
case InventoryLocationType.ATTACHMENT:
{
res = res + " item=" + Object.GetDebugName(GetItem());
res = res + " parent=" + Object.GetDebugName(GetParent());
res = res + " slot=" + GetSlot();
break;
}
case InventoryLocationType.CARGO:
{
res = res + " item=" + Object.GetDebugName(GetItem());
res = res + " parent=" + Object.GetDebugName(GetParent());
res = res + " idx=" + GetIdx() + " row=" + GetRow() + " col=" + GetCol() + " f=" + GetFlip();
break;
}
case InventoryLocationType.HANDS:
{
res = res + " item=" + Object.GetDebugName(GetItem());
res = res + " parent=" + Object.GetDebugName(GetParent());
break;
}
case InventoryLocationType.PROXYCARGO:
{
res = res + " item=" + Object.GetDebugName(GetItem());
res = res + " parent=" + Object.GetDebugName(GetParent());
res = res + " idx=" + GetIdx() + " row=" + GetRow() + " col=" + GetCol() + " f=" + GetFlip();
break;
}
case InventoryLocationType.VEHICLE:
{
res = res + " item=" + Object.GetDebugName(GetItem());
res = res + " parent=" + Object.GetDebugName(GetParent());
res = res + " idx=" + GetIdx();
break;
}
case InventoryLocationType.TEMP:
{
res = res + " item=" + Object.GetDebugName(GetItem());
res = res + " parent=" + Object.GetDebugName(GetParent());
break;
}
default:
{
res = res + "??";
break;
}
}
res = res + " }";
return res;
}
bool ReadFromContext(ParamsReadContext ctx)
{
EntityAI parent;
EntityAI item;
int type = 0;
int idx = -1;
int row = -1;
int col = -1;
bool flp = false;
if (!ctx.Read(type))
return false;
switch (type)
{
case InventoryLocationType.UNKNOWN:
{
break;
}
case InventoryLocationType.GROUND:
{
if (!ctx.Read(item))
return false;
vector pos;
if (!ctx.Read(pos))
return false;
float dir[4];
if (!ctx.Read(dir))
return false;
if (!item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Item=%1 does not exist on server!", Object.GetDebugName(item)), "GROUND" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break; // parent or item is possibly not spawned in bubble yet
}
SetGroundEx(item, pos, dir);
break;
}
case InventoryLocationType.ATTACHMENT:
{
if (!ctx.Read(parent))
return false;
if (!ctx.Read(item))
return false;
int slot;
if (!ctx.Read(slot))
return false;
if (!parent || !item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Parent=%1 or Item=%2 does not exist on server!", Object.GetDebugName(parent), Object.GetDebugName(item)), "ATTACHMENT" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break; // parent or item is possibly not spawned in bubble yet
}
SetAttachment(parent, item, slot);
break;
}
case InventoryLocationType.CARGO:
{
if (!ctx.Read(parent))
return false;
if (!ctx.Read(item))
return false;
if (!ctx.Read(idx))
return false;
if (!ctx.Read(row))
return false;
if (!ctx.Read(col))
return false;
if (!ctx.Read(flp))
return false;
if (!parent || !item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Parent=%1 or Item=%2 does not exist on server!", Object.GetDebugName(parent), Object.GetDebugName(item)), "CARGO" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break; // parent or item is possibly not spawned in bubble yet
}
SetCargo(parent, item, idx, row, col, flp);
break;
}
case InventoryLocationType.HANDS:
{
if (!ctx.Read(parent))
return false;
if (!ctx.Read(item))
return false;
if (!parent || !item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Parent=%1 or Item=%2 does not exist on server!", Object.GetDebugName(parent), Object.GetDebugName(item)), "HANDS" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break; // parent or item is possibly not spawned in bubble yet
}
SetHands(parent, item);
break;
}
case InventoryLocationType.PROXYCARGO:
{
if (!ctx.Read(parent))
return false;
if (!ctx.Read(item))
return false;
if (!ctx.Read(idx))
return false;
if (!ctx.Read(row))
return false;
if (!ctx.Read(col))
return false;
if (!ctx.Read(flp))
return false;
if (!parent || !item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Parent=%1 or Item=%2 does not exist on server!", Object.GetDebugName(parent), Object.GetDebugName(item)), "PROXYCARGO" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break; // parent or item is possibly not spawned in bubble yet
}
SetProxyCargo(parent, item, idx, row, col, flp);
break;
}
case InventoryLocationType.VEHICLE:
{
if (!ctx.Read(parent))
return false;
if (!ctx.Read(item))
return false;
if (!ctx.Read(idx))
return false;
if (!parent || !item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Parent=%1 or Item=%2 does not exist on server!", Object.GetDebugName(parent), Object.GetDebugName(item)), "VEHICLE" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break; // parent or item is possibly not spawned in bubble yet
}
SetVehicle(parent, item, idx);
break;
}
case InventoryLocationType.TEMP:
{
if (!ctx.Read(parent))
return false;
if (!ctx.Read(item))
return false;
int tmp;
if (!ctx.Read(tmp))
return false;
if (!parent || !item)
{
#ifdef ENABLE_LOGGING
#ifdef SERVER
Debug.Log(string.Format("Parent=%1 or Item=%2 does not exist on server!", Object.GetDebugName(parent), Object.GetDebugName(item)), "TEMP" , "n/a", "ReadFromContext", this.ToString() );
#endif
#endif
break;
}
SetAttachment(parent, item, -1);
//SetTemporary(parent, item);
}
default:
{
ErrorEx("ReadFromContext - really unknown location type, this should not happen, type=" + type);
return false;
}
}
return true;
}
bool WriteToContext(ParamsWriteContext ctx)
{
if (GetType() == InventoryLocationType.TEMP)
{
int type = InventoryLocationType.ATTACHMENT;
if (!ctx.Write(type))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed to write temp type");
return false;
}
}
else if (!ctx.Write(GetType()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed to write type");
return false;
}
switch (GetType())
{
case InventoryLocationType.UNKNOWN:
{
break;
}
case InventoryLocationType.GROUND:
{
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed GND, arg=item");
return false;
}
vector pos = GetPos();
if (!ctx.Write(pos))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed GND, arg=pos");
return false;
}
float dir[4];
GetDir(dir);
if (!ctx.Write(dir))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed GND, arg=dir");
return false;
}
break;
}
case InventoryLocationType.ATTACHMENT:
{
if (!ctx.Write(GetParent()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed ATT, arg=parent");
return false;
}
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed ATT, arg=item");
return false;
}
if (!ctx.Write(GetSlot()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed ATT, arg=slot");
return false;
}
break;
}
case InventoryLocationType.CARGO:
{
if (!ctx.Write(GetParent()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed CGO, arg=parent");
return false;
}
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed CGO, arg=item");
return false;
}
if (!ctx.Write(GetIdx()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed CGO, arg=idx");
return false;
}
if (!ctx.Write(GetRow()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed CGO, arg=row");
return false;
}
if (!ctx.Write(GetCol()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed CGO, arg=col");
return false;
}
if (!ctx.Write(GetFlip()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed CGO, arg=flp");
return false;
}
break;
}
case InventoryLocationType.HANDS:
{
if (!ctx.Write(GetParent()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed HND, arg=parent");
return false;
}
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed HND, arg=item");
return false;
}
break;
}
case InventoryLocationType.PROXYCARGO:
{
if (!ctx.Write(GetParent()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed PXY, arg=parent");
return false;
}
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed PXY, arg=item");
return false;
}
if (!ctx.Write(GetIdx()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed PXY, arg=idx");
return false;
}
if (!ctx.Write(GetRow()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed PXY, arg=row");
return false;
}
if (!ctx.Write(GetCol()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed PXY, arg=col");
return false;
}
if (!ctx.Write(GetFlip()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed PXY, arg=flp");
return false;
}
break;
}
case InventoryLocationType.VEHICLE:
{
if (!ctx.Write(GetParent()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed VHC, arg=parent");
return false;
}
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed VHC, arg=item");
return false;
}
if (!ctx.Write(GetIdx()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed VHC, arg=idx");
return false;
}
break;
}
case InventoryLocationType.TEMP:
{
if (!ctx.Write(GetParent()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed TMP, arg=parent");
return false;
}
if (!ctx.Write(GetItem()))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed TMP, arg=item");
return false;
}
if (!ctx.Write(-1))
{
Error("InventoryLocation::WriteToContext - cannot write to context! failed TMP, idx=-1");
return false;
}
break;
}
default:
{
Error("WriteToContext - really unknown location type, this should not happen, type=" + GetType());
return false;
}
}
return true;
}
};
bool OptionalLocationWriteToContext(InventoryLocation loc, notnull ParamsWriteContext ctx)
{
if (loc)
{
if (!ctx.Write(true))
{
Error("OptionalLocationWriteToContext - cannot write 1 to context!");
return false;
}
return loc.WriteToContext(ctx);
}
else
{
if (!ctx.Write(false))
{
Error("OptionalLocationWriteToContext - cannot write 0 to context!");
return false;
}
}
return true;
}
bool OptionalLocationReadFromContext(out InventoryLocation loc, notnull ParamsReadContext ctx)
{
bool present = false;
if (!ctx.Read(present))
{
Error("OptionalLocationReadFromContext - cannot read bool from context!");
return false;
}
if (!present)
return true;
loc = new InventoryLocation();
if (!loc.ReadFromContext(ctx))
{
Error("OptionalLocationReadFromContext - cannot read (present) inventorylocation from context!");
return false;
}
return true;
}