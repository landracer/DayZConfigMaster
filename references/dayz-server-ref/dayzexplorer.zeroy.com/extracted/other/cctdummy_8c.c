// ======================================================================
// File: cctdummy_8c_source.html
// Category: other
// ======================================================================

class CCTDummy : CCTBase
{
override bool Can( PlayerBase player, ActionTarget target )
{
return ( target && target.GetObject() );
}
};