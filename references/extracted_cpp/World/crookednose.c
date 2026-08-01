// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/crookednose.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CrookedNose extends Mask_Base
{
    override protected void InitGlobalExclusionValues()
    {
        super.InitGlobalExclusionValues();
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_1);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_2);
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_HEADGEAR_HELMET_0);
