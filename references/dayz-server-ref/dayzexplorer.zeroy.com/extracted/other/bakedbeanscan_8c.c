// ======================================================================
// File: bakedbeanscan_8c_source.html
// Category: other
// ======================================================================

class BakedBeansCan : Edible_Base
{
override void Open()
{
ReplaceEdibleWithNew("BakedBeansCan_Opened");
}
override float GetQuantityNormalizedScripted()
{
return 1.0;
}
}