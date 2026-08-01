// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/mimemask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class MimeMask_Male : Mask_Base
{
    override protected void InitGlobalExclusionValues()
    {
        super.InitGlobalExclusionValues();
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_1);
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_TIGHT_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_REGULAR_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_HEADGEAR_HELMET_0);
