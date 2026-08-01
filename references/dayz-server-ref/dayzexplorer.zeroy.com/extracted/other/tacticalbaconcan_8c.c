// ======================================================================
// File: tacticalbaconcan_8c_source.html
// Category: other
// ======================================================================

class TacticalBaconCan : Edible_Base
{
override void Open()
{
ReplaceEdibleWithNew("TacticalBaconCan_Opened");
}
override bool IsOpen()
{
return false;
}
override float GetQuantityNormalizedScripted()
{
return 1.0;
}
}