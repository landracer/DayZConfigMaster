// ======================================================================
// File: chainmail__coif_8c_source.html
// Category: other
// ======================================================================

class Chainmail_Coif extends HeadGear_Base
{
override protected set<int> GetAttachmentExclusionInitSlotValue(int slotId)
{
set<int> ret = super.GetAttachmentExclusionInitSlotValue(slotId);
if (slotId == InventorySlots.HEADGEAR)
{
ret.Insert(EAttExclusions.LEGACY_MASK_HEADGEAR); //same as &#39;noMask&#39; used to be
ret.Insert(EAttExclusions.SHAVING_HEADGEAR_ATT_0);
}
return ret;
}
};