// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/container_base/en5c_waterproofbag_colorbase.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class WaterproofBag_ColorBase : Container_Base
{
    override bool CanPutInCargo(EntityAI parent)
    {
        if (!super.CanPutInCargo(parent))
            return false;
        
        if (parent && (parent.IsKindOf(&quot;AmmoBox&quot;) || parent.IsKindOf(&quot;WaterproofBag_ColorBase&quot;)))
            return false;
        
        return true;
