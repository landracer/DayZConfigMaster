// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/paydaymask_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class PaydayMask_ColorBase : Mask_Base
{
    override protected set<int> GetAttachmentExclusionInitSlotValue(int slotId)
    {
        set<int> ret = super.GetAttachmentExclusionInitSlotValue(slotId);
        if (slotId == InventorySlots.MASK)
        {
            ret.Insert(EAttExclusions.LEGACY_HEADGEAR_MASK); //same as &#39;noHeadgear&#39; used to be
        }
        return ret;
