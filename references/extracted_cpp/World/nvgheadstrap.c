// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/clothing/nvgheadstrap.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class NVGHeadstrap : Glasses_Base
{
    override void SetActions()
    {
        super.SetActions();
        
        AddAction(ActionToggleNVG);
