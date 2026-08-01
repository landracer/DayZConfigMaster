// ======================================================================
// File: handdrillkit_8c_source.html
// Category: other
// ======================================================================

class HandDrillKit extends ItemBase
{
override bool CanIgniteItem( EntityAI ignite_target = NULL )
{
return true;
}
override void OnIgnitedTarget( EntityAI target_item )
{
if ( g_Game.IsServer() )
{
DecreaseHealth( 20 );
}
}
override void OnIgnitedTargetFailed( EntityAI target_item )
{
if ( g_Game.IsServer() )
{
DecreaseHealth( 20 );
}
}
override void SetActions()
{
super.SetActions();
AddAction(ActionLightItemOnFire);
}
/*
override bool IsTargetIgnitionSuccessful( EntityAI item_target = NULL )
{
}
*/
}