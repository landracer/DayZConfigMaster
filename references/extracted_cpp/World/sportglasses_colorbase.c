// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/sportglasses_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SportGlasses_ColorBase extends Glasses_Base 
{
    override protected void InitGlobalExclusionValues()
    {
        super.InitGlobalExclusionValues();
        ClearSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_REGULAR_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_TIGHT_0);
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_GLASSES_REGULAR_0); //TODO: check consistent mask conflict
