// ======================================================================
// File: ccinotruinedandempty_8c_source.html
// Category: other
// ======================================================================

class CCINotRuinedAndEmpty : CCIBase
{
override bool Can( PlayerBase player, ItemBase item )
{
return ( item && !item.IsDamageDestroyed() && item.GetQuantity() > 0);
}
};