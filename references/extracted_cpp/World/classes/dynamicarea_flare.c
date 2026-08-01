// Source: F:/Games/Dayz/scripts/4_world/classes/contaminatedarea/dynamicarea_flare.c Source File
// Extracted from DayZ Code Explorer by Zeroy

// The dummy entity which is spawned in the announcement process of Dynamic Contaminated Areas
class DynamicArea_Flare : ScriptedEntity
{
    const float FALL_SPEED = 0.5; // The speed at which this object falls ( units per second )
    const float LIFETIME = 40000; // How long this entity will stay in world
    private Particle m_Particle;
    void DynamicArea_Flare()
    {
        SetEventMask( EntityEvent.FRAME );
        SetFlags( EntityFlags.ACTIVE, false );
        
        // We create the particle effect
        m_Particle = ParticleManager.GetInstance().PlayOnObject( ParticleList.FLAREPROJ_ACTIVATE_RED, this );
        
        // We specify we will delete this object after set time
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( g_Game.ObjectDeleteOnClient, LIFETIME, false, this );
