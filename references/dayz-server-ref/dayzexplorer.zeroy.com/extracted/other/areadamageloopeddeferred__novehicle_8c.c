// ======================================================================
// File: areadamageloopeddeferred__novehicle_8c_source.html
// Category: other
// ======================================================================

class AreaDamageLoopedDeferred_NoVehicle: AreaDamageLoopedDeferred
{
override bool OnEvaluateDamageEx( TriggerInsider insider, float deltaTime )
{
// For case where player is inside vehicle
if ( insider.GetObject().IsMan() )
{
PlayerBase playerInsider = PlayerBase.Cast( insider.GetObject() );
if ( playerInsider.GetCommand_Vehicle() )
return false;
}
return super.OnEvaluateDamageEx( insider, deltaTime );
}
}