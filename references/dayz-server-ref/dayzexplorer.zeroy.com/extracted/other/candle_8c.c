// ======================================================================
// File: candle_8c_source.html
// Category: other
// ======================================================================

class Candle extends Inventory_Base
{
override bool CanPutInCargo( EntityAI parent )
{
if( !super.CanPutInCargo(parent) ) {return false;}
if (parent && !parent.HasSelection("flame"))
{
return true;
}
return false;
}
}