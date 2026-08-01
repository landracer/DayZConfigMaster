// Source: F:/Games/Dayz/scripts/4_world/classes/areadamage/areadamagenew/areadamageloopeddeferred_novehicle.c Source File
// Extracted from DayZ Code Explorer by Zeroy


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
