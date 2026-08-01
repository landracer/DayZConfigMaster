// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/smokesimulation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class SmokeSimulation : Managed
{
    protected Particle          m_ParMainSmoke;
    int particle_id;
    
    void SmokeSimulation()
    {
        particle_id = ParticleList.GRENADE_M18_PURPLE_LOOP;
