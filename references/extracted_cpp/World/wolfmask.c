// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/wolfmask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WolfMask extends ClothingBase
{
    override protected void InitGlobalExclusionValues()
    {
        super.InitGlobalExclusionValues();
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_1);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_2);
