// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/chainmail_coif.c Source File
// Extracted from DayZ Code Explorer by Zeroy

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
