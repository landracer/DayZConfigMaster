// ======================================================================
// File: peachescan_8c_source.html
// Category: other
// ======================================================================

class PeachesCan : Edible_Base
{
override void Open()
{
ReplaceEdibleWithNew("PeachesCan_Opened");
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