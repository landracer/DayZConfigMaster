// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/headdress_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class Headdress_Colorbase extends HeadGear_Base
{
    override protected void InitGlobalExclusionValues()
    {
        super.InitGlobalExclusionValues();
        
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_HEADSTRAP_0);
        AddSingleExclusionValueGlobal(EAttExclusions.EXCLUSION_MASK_2);
