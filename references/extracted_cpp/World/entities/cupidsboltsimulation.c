// Source: F:/Games/Dayz/scripts/4_world/entities/firearms/cupidsboltsimulation.c Source File
// Extracted from DayZ Code Explorer by Zeroy

class CupidsBoltSimulation : Managed
{
    protected Particle m_ParticleTail;
    
    void OnFire(Entity entity)
    {
        if (m_ParticleTail)
            m_ParticleTail.Stop();
 
        m_ParticleTail = ParticleManager.GetInstance().PlayOnObject(ParticleList.BOLT_CUPID_TAIL, entity);
        m_ParticleTail.SetWiggle(7, 0.3);
