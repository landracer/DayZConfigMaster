// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/hockeymask.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class HockeyMask extends Mask_Base
{
    override protected void InitGlobalExclusionValues()
    {
        super.InitGlobalExclusionValues();
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_0);
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_TIGHT_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_REGULAR_0);
