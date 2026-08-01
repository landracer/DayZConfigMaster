// ======================================================================
// File: ccidummy_8c_source.html
// Category: other
// ======================================================================

class CCIDummy : CCIBase
{
override bool Can( PlayerBase player, ItemBase item )
{
return item != null;
}
};