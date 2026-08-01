// ======================================================================
// File: spaghettican_8c_source.html
// Category: other
// ======================================================================

class SpaghettiCan : Edible_Base
{
override void Open()
{
ReplaceEdibleWithNew("SpaghettiCan_Opened");
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