// ======================================================================
// File: inventoryslots_8c_source.html
// Category: other
// ======================================================================

class InventorySlots
{
const int COUNT;
const int INVALID;
#ifdef DIAG_DEVELOPER
private void InventorySlots() {}
private void ~InventorySlots() {}
#else
void InventorySlots() {}
void ~InventorySlots() {}
#endif
static proto native int GetSlotIdFromString(string slot_name);
static proto native owned string GetSlotName(int id);
static proto native owned string GetSlotDisplayName(int id);
static proto native bool IsSlotIdValid(int slotId);
static proto native int GetStackMaxForSlotId(int slot_Id);
static proto native bool GetShowForSlotId(int slot_Id);
static proto native bool GetAutoAttachForSlotId(int slot_Id);
static proto bool GetBoneNameForSlotId(int slot_Id, out string bone_name);
static proto bool GetSelectionForSlotId(int slot_Id, out string selection);
//static proto native int GetBoneIndexForSlotId (int slot_Id);
};