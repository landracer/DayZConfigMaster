// ======================================================================
// File: ccinonruined_8c_source.html
// Category: other
// ======================================================================

class CCINonRuined : CCIBase
{
override bool Can( PlayerBase player, ItemBase item )
{
return ( item && !item.IsDamageDestroyed() );
}
};