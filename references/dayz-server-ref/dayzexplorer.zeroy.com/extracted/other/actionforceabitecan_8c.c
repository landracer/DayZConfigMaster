// ======================================================================
// File: actionforceabitecan_8c_source.html
// Category: other
// ======================================================================

class ActionForceABiteCan: ActionForceABite
{
override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
{
return true;
/*
//if staging for cans is introduced
if ( item && item.IsOpen() )
{
return true;
}
return false;
*/
}
};