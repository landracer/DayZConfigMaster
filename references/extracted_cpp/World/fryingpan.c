// Source: F:/Games/Dayz/scripts/4_world/entities/itembase/inventory_base/fryingpan.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class FryingPan : Inventory_Base
{
    // Cooking data
    protected CookingMethodType         m_CookingMethod;
    protected bool                      m_CookingIsDone;
    protected bool                      m_CookingIsEmpty;
    protected bool                      m_CookingIsBurned;
 
    // Particles
    protected Particle  m_ParticleCooking;
    protected int       m_ParticlePlaying   = ParticleList.INVALID;
 
    protected int PARTICLE_BAKING_START     = ParticleList.COOKING_BAKING_START;
    protected int PARTICLE_BAKING_DONE      = ParticleList.COOKING_BAKING_DONE;
    protected int PARTICLE_DRYING_START     = ParticleList.COOKING_DRYING_START;
    protected int PARTICLE_DRYING_DONE      = ParticleList.COOKING_DRYING_DONE;
    protected int PARTICLE_BURNING_DONE     = ParticleList.COOKING_BURNING_DONE;
 
    void FryingPan()
    {
        RegisterNetSyncVariableInt( &quot;m_CookingMethod&quot;, CookingMethodType.NONE, CookingMethodType.COUNT );
        RegisterNetSyncVariableBool( &quot;m_CookingIsDone&quot; );
        RegisterNetSyncVariableBool( &quot;m_CookingIsEmpty&quot; );
        RegisterNetSyncVariableBool( &quot;m_CookingIsBurned&quot; );
