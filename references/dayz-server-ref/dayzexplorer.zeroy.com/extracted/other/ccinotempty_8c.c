// ======================================================================
// File: ccinotempty_8c_source.html
// Category: other
// ======================================================================

class CCINotEmpty : CCIBase
{
override bool Can( PlayerBase player, ItemBase item )
{
return ( item && item.GetQuantity() > 0 );
}
};