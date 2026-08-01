// ======================================================================
// File: sardinescan_8c_source.html
// Category: other
// ======================================================================

class SardinesCan : Edible_Base
{
override void Open()
{
ReplaceEdibleWithNew("SardinesCan_Opened");
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