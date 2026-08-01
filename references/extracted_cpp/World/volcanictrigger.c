// Source: F:/Games/Dayz/scripts/4_world/entities/scriptedentities/triggers/volcanictrigger.c Source File
// Extracted from DayZ Code Explorer by Zeroy


class VolcanicTrigger : EffectTrigger
{   
    static VolcanicTrigger m_SourceDamageInstance; // this is used only server side for logging damage source in admin log
    static const float HEAT_DAMAGE_MULTIPLIER = 5;
    static const float HEAT_DAMAGE_TICK_TIME = 2;
    
    protected ParticleSource m_VaporParticle; 
        
    override void EEInit()
    {
        if (g_Game.IsServer() && !m_SourceDamageInstance)
            m_SourceDamageInstance = this;
        
        super.EEInit();
                
        #ifndef SERVER
        SpawnVaporEffect();
        #endif
