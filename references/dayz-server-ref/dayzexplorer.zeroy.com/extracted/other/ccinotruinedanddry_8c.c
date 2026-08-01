// ======================================================================
// File: ccinotruinedanddry_8c_source.html
// Category: other
// ======================================================================

class CCINotRuinedAndDry extends CCIBase
{
override bool Can( PlayerBase player, ItemBase item )
{
return ( item && !item.IsDamageDestroyed() && item.GetWet() <= 0 ) ;
}
};