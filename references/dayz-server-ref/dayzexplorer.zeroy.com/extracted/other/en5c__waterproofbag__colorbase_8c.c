// ======================================================================
// File: en5c__waterproofbag__colorbase_8c_source.html
// Category: other
// ======================================================================

class WaterproofBag_ColorBase : Container_Base
{
override bool CanPutInCargo(EntityAI parent)
{
if (!super.CanPutInCargo(parent))
return false;
if (parent && (parent.IsKindOf("AmmoBox") || parent.IsKindOf("WaterproofBag_ColorBase")))
return false;
return true;
}
}
class WaterproofBag_Yellow : WaterproofBag_ColorBase {};
class WaterproofBag_Green : WaterproofBag_ColorBase {};
class WaterproofBag_Orange : WaterproofBag_ColorBase {};